#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <string>

#include "midi.hpp"

typedef std::basic_string<unsigned char> ByteString;
std::vector<std::string> INSTRUMENT_NAMES = { "Kick", "Snare", "Closed Hi-hat", "Open Hi-hat" };

const unsigned char STEP_SIZE = 4; // size of a 16th note
const unsigned char NUM_STEPS = 16; // number of 16th notes

const unsigned char get_seq_size() {
    return NUM_STEPS;
}

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

std::basic_string<unsigned char> MIDISequence::writeToBuffer() {
    // Time Signature
    ByteString track = hexToByteString("00FF580404022401");

    std::vector<Event> currentEvents{};
    char lastStep = 0;

    sort();
    for (const Event& ev : events) {

        unsigned char deltaTime = (ev.t - lastStep) * STEP_SIZE;
        lastStep = ev.t;

        // if deltaTime is real, then note OFF!
        if (deltaTime > 0) {
            unsigned char newDeltaTime = deltaTime - STEP_SIZE;
            deltaTime = STEP_SIZE;
            if (currentEvents.empty()) newDeltaTime += STEP_SIZE;
            for (const Event& cev : currentEvents) {
                track += deltaTime;
                deltaTime = 0;
                track += 0x80; // NOTE OFF
                track += static_cast<unsigned char>(cev.instr)+36; // KEY
                track += 0x64; // VEL
            }
            currentEvents.clear();
            deltaTime = newDeltaTime;
        }
        currentEvents.push_back(ev);

        track += deltaTime;
        deltaTime = 0;
        track += 0x90; // NOTE ON
        track += static_cast<unsigned char>(ev.instr)+36; // KEY
        track += 0x64; // VEL
    }

    unsigned char deltaTime = ((NUM_STEPS-1) - lastStep) * STEP_SIZE;

    unsigned char newDeltaTime = deltaTime - STEP_SIZE;
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

    ByteString output = {'M', 'T', 'h', 'd'};
    output += hexToByteString("00000006000000010010"); // file info TODO: put STEP_SIZE here instead of 0x10
    output += {'M', 'T', 'r', 'k'};

    int ts = track.size();
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