#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <filesystem>
#include <string>

#include "midi.hpp"

// -----------------------------
// Constants
// -----------------------------

const std::map<MIDISequence::Byte, std::string> instrument_names = {
    {36, "Kick"}, {37, "Snare"}, {38, "Closed Hi-hat"}, {39, "Open Hi-hat"} };

const MIDISequence::Byte note_on = 0x90;
const MIDISequence::Byte note_off = 0x80;
const MIDISequence::Byte default_velocity = 0x64;

const MIDISequence::ByteString time_signature = MIDISequence::hexToByteString("00FF580404022401");
// TODO: make header include step_size info, rn it's hardcoded at qnote = 16 ticks
const MIDISequence::ByteString header = MIDISequence::hexToByteString("4D546864000000060000000100104D54726B");
const MIDISequence::ByteString track_end = MIDISequence::hexToByteString("FF2F00");

// -----------------------------
// MIDISequence Definitions
// -----------------------------

MIDISequence::MIDISequence(): events{} {};

MIDISequence::MIDISequence(size_t num_steps): events{}, num_steps{num_steps} {};

void MIDISequence::addEvent(MIDITick t, Byte message, Byte instr) {
    MIDISequence::Event event{t, message, instr};
    events.push_back(event);
}

void MIDISequence::addNote(MIDITick t, MIDITick len, Byte instr) {
    this->addEvent(t, note_on, instr);
    this->addEvent(t+len, note_off, instr);
}

std::vector<MIDISequence::Byte> MIDISequence::writeToBuffer() {
    ByteString track = time_signature;

    this->sort();

    MIDITick current_tick = 0;
    MIDITick delta_tick = 0;

    for (const Event& ev : this->events) {
        delta_tick = ev.t - current_tick;
        current_tick = ev.t;

        ByteString delta_tick_vlq = makeVariableLengthQuantity(delta_tick);
        track.insert(track.end(), delta_tick_vlq.begin(), delta_tick_vlq.end());
        track.push_back(ev.message);
        track.push_back(ev.instr);
        track.push_back(default_velocity);
    }

    delta_tick = (step_size * num_steps) - current_tick;

    ByteString delta_tick_vlq = makeVariableLengthQuantity(delta_tick);
    track.insert(track.end(), delta_tick_vlq.begin(), delta_tick_vlq.end());

    track.insert(track.end(), track_end.begin(), track_end.end());

    ByteString output = header;
    ByteString track_size = makeVariableLengthQuantity(track.size());

    while (track_size.size() < 4) {
        track_size.insert(track_size.begin(), 0);
    }

    output.insert(output.end(), track_size.begin(), track_size.end());
    output.insert(output.end(), track.begin(), track.end());

    return output;
}

bool MIDISequence::writeToFile(std::string p) {
    // Open file
    std::ofstream f{p, std::ios::binary};
    
    ByteString output = this->writeToBuffer();
    writeByteString(f, output);

    return true;
}

std::string MIDISequence::to_string() const {
    std::string result = "MIDISequence Length: " + std::to_string(this->events.size()) + "\n";
    for (const Event& ev : this->events) {
        std::string instr_name = instrument_names.at(ev.instr);
        std::string message_name = (ev.message == note_on) ? "note_on" : "note_off";
        result += "\tt: " + std::to_string(ev.t);
        result += "\tmsg: " + message_name;
        result += "\tinstr: " + instr_name + "\n";
    }
    return result;
}

void MIDISequence::append(const MIDISequence& seq) {
    for (const Event& ev : seq.events) {
        this->addEvent(ev.t, ev.message, ev.instr);
    }
}

void MIDISequence::sort() {
    std::sort(events.begin(), events.end(), [](const Event& a, const Event& b) {
        if (a.t != b.t) {
            return a.t < b.t; 
        } else {
            return a.message == note_off;
        }
    });
}

std::ostream& operator<<(std::ostream& os, const MIDISequence& seq) {
    return os << seq.to_string();
}

// -----------------------------
// Helper Definitions
// -----------------------------

MIDISequence::ByteString MIDISequence::hexToByteString(const std::string& hex) {
    ByteString bs{};
    for (size_t i = 0; i < hex.length(); i += 2) {
        uint8_t byte = std::stoi(hex.substr(i, 2), 0, 16);
        bs.push_back(byte);
    }
    return bs;
}

void MIDISequence::writeByteString(std::ofstream& f, ByteString bs) {
    f.write(reinterpret_cast<const char*>(bs.data()), bs.size());
}

MIDISequence::ByteString MIDISequence::makeVariableLengthQuantity(MIDITick val) {
    ByteString vlq = {};
    while (val != 0) {
        vlq.insert(vlq.begin(), (val & 0x7F));
        val >>= 7;
    }
    if (vlq.size() == 0) vlq.push_back(0);
    return vlq;
}