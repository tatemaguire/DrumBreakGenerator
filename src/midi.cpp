#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <string>

#include "midi.hpp"

typedef std::string ByteString;
const char STEP_SIZE = 1;
std::vector<std::string> INSTRUMENT_NAMES = { "Kick", "Snare", "Closed Hi-hat", "Open Hi-hat" };

// -----------------------------
// Helper Function Declaration
// -----------------------------

ByteString hexToByteString(const std::string& hex);
void writeByteString(std::ofstream& f, ByteString bs);

// -----------------------------
// MIDISequence Function Definitions
// -----------------------------

MIDISequence::MIDISequence(): events{} {};

void MIDISequence::addEvent(char t, Instrument instr) {
    MIDISequence::Event event{t, instr};
    events.push_back(event);
}

std::string MIDISequence::writeToBuffer() {
    // Time Signature
    ByteString track = hexToByteString("00FF580404022401");

    std::vector<Event> currentEvents{};
    char lastStep = 0;

    sort();
    for (const Event& ev : events) {

        char deltaTime = (ev.t - lastStep) * STEP_SIZE;
        lastStep = ev.t;

        // if deltaTime is real, then note OFF!
        if (deltaTime > 0) {
            char newDeltaTime = deltaTime - STEP_SIZE;
            deltaTime = STEP_SIZE;
            if (currentEvents.empty()) newDeltaTime += STEP_SIZE;
            for (const Event& cev : currentEvents) {
                track += deltaTime;
                deltaTime = 0;
                track += 0x80; // NOTE OFF
                track += static_cast<char>(cev.instr)+36; // KEY
                track += 0x64; // VEL
            }
            currentEvents.clear();
            deltaTime = newDeltaTime;
        }
        currentEvents.push_back(ev);

        track += deltaTime;
        deltaTime = 0;
        track += 0x90; // NOTE ON
        track += static_cast<char>(ev.instr)+36; // KEY
        track += 0x64; // VEL
    }

    char deltaTime = ((SEQ_SIZE-1) - lastStep) * STEP_SIZE;

    char newDeltaTime = deltaTime - STEP_SIZE;
    deltaTime = STEP_SIZE;
    if (currentEvents.empty()) newDeltaTime += STEP_SIZE;
    for (const Event& cev : currentEvents) {
        track += deltaTime;
        deltaTime = 0;
        track += 0x80; // NOTE OFF
        track += static_cast<char>(cev.instr)+36; // KEY
        track += 0x64; // VEL
    }
    deltaTime = newDeltaTime;
    deltaTime = 0;

    track += deltaTime;
    track += hexToByteString("FF2F00"); // end of clip

    ByteString output = "MThd";
    output += hexToByteString("0000000600000001004"); // file info
    output += "MTrk";

    int ts = track.size();
    char dig1 = ts % 256;
    char dig2 = ts / 256;
    
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
        result += "\tt: " + std::to_string(ev.t) + "\tinstr: " + INSTRUMENT_NAMES[ev.instr] + "\n";
    }
    return result;
}

void MIDISequence::append(const MIDISequence& seq) {
    for (const Event& ev : seq.events) {
        this->addEvent(ev.t, ev.instr);
    }
}

void MIDISequence::sort() {
    std::sort(events.begin(), events.end(), [](const Event& a, const Event& b) { return a.t < b.t; });
}

std::ostream& operator<<(std::ostream& os, const MIDISequence& seq) {
    return os << seq.to_string();
}

// -----------------------------
// Helper Functions
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