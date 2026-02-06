#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <string>

#include "midi.hpp"

// -----------------------------
// Helper Declarations
// -----------------------------

typedef std::basic_string<unsigned char> ByteString;

ByteString hexToByteString(const std::string& hex);
void writeByteString(std::ofstream& f, ByteString bs);

// -----------------------------
// Constants
// -----------------------------

const std::vector<std::string> INSTRUMENT_NAMES = { "Kick", "Snare", "Closed Hi-hat", "Open Hi-hat" };

const unsigned char STEP_SIZE = 4; // size of a 16th note
const unsigned char NUM_STEPS = 16; // number of 16th notes

const unsigned char NOTE_ON = 0x90;
const unsigned char NOTE_OFF = 0x80;
const unsigned char DEFAULT_VELOCITY = 0x64;

const ByteString TIME_SIGNATURE = hexToByteString("00FF580404022401");
// TODO: make HEADER include STEP_SIZE info, rn it's hardcoded at qnote = 16 ticks
const ByteString HEADER = hexToByteString("4D546864000000060000000100104D54726B");
const ByteString TRACK_END = hexToByteString("FF2F00");

// TODO: remove this fugly mess
const unsigned char get_seq_size() {
    return NUM_STEPS;
}

// -----------------------------
// MIDISequence Definitions
// -----------------------------

MIDISequence::MIDISequence(): events{} {};

void MIDISequence::addEvent(char t, unsigned char message, Instrument instr) {
    MIDISequence::Event event{t, message, instr};
    events.push_back(event);
}

void MIDISequence::addNote(char t, char len, Instrument instr) {
    this->addEvent(t, NOTE_ON, instr);
    this->addEvent(t+len, NOTE_OFF, instr);
}

std::basic_string<unsigned char> MIDISequence::writeToBuffer() {
    ByteString track = TIME_SIGNATURE;

    this->sort();

    unsigned char currentTick = 0;
    unsigned char deltaTick = 0;

    for (const Event& ev : this->events) {
        deltaTick = ev.t - currentTick;
        currentTick = ev.t;

        track += deltaTick;
        track += ev.message;
        track += static_cast<char>(ev.instr)+36;
        track += DEFAULT_VELOCITY;
    }

    deltaTick = (STEP_SIZE * NUM_STEPS) - currentTick;

    track += deltaTick;
    track += TRACK_END;

    ByteString output = HEADER;

    // TODO: make a better way of making variable length quantity
    size_t ts = track.size();
    unsigned char dig1 = ts % 256;
    unsigned char dig2 = ts / 256;

    ByteString trackSize{0, 0, dig2, dig1};

    output += trackSize;
    output += track;

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
    std::string result = "MIDISequence:\n";
    for (const Event& ev : events) {
        std::string instrName = INSTRUMENT_NAMES[static_cast<int>(ev.instr)];
        result += "\tt: " + std::to_string(ev.t) + "\tinstr: " + instrName + "\n";
    }
    return result;
}

void MIDISequence::append(const MIDISequence& seq) {
    for (const Event& ev : seq.events) {
        this->addEvent(ev.t, ev.message, ev.instr);
    }
}

void MIDISequence::sort() {
    std::sort(events.begin(), events.end(), [](const Event& a, const Event& b) { return a.t < b.t; });
}

std::ostream& operator<<(std::ostream& os, const MIDISequence& seq) {
    return os << seq.to_string();
}

// -----------------------------
// Helper Definitions
// -----------------------------

ByteString hexToByteString(const std::string& hex) {
    ByteString bs{};
    for (size_t i = 0; i < hex.length(); i += 2) {
        char byte = std::stoi(hex.substr(i, 2), 0, 16);
        bs.push_back(byte);
    }
    return bs;
}

void writeByteString(std::ofstream& f, ByteString bs) {
    f.write(reinterpret_cast<const char*>(bs.data()), bs.size());
}