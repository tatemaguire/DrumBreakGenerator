#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

#include "midi.hpp"

typedef std::basic_string<uint8_t> ByteString;
const uint8_t STEP_SIZE = 1;
const uint8_t SEQ_SIZE = 16;

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

bool MIDISequence::writeToFile(std::string p) {
    // Time Signature
    ByteString track = hexToByteString("00FF580404022401");

    std::vector<Event> currentEvents{};
    uint8_t lastStep = 0;

    sort();
    for (const Event& ev : events) {

        uint8_t deltaTime = (ev.t - lastStep) * STEP_SIZE;
        lastStep = ev.t;

        // if deltaTime is real, then note OFF!
        if (deltaTime > 0) {
            uint8_t newDeltaTime = deltaTime - STEP_SIZE;
            deltaTime = STEP_SIZE;
            if (currentEvents.empty()) newDeltaTime += STEP_SIZE;
            for (const Event& cev : currentEvents) {
                track += deltaTime;
                deltaTime = 0;
                track += 128; // NOTE OFF
                track += static_cast<uint8_t>(cev.instr)+36; // KEY
                track += 100; // VEL
            }
            currentEvents.clear();
            deltaTime = newDeltaTime;
        }
        currentEvents.push_back(ev);

        track += deltaTime;
        deltaTime = 0;
        track += 144; // NOTE ON
        track += static_cast<uint8_t>(ev.instr)+36; // KEY
        track += 100; // VEL
    }

    uint8_t deltaTime = ((SEQ_SIZE-1) - lastStep) * STEP_SIZE;

    uint8_t newDeltaTime = deltaTime - STEP_SIZE;
    deltaTime = STEP_SIZE;
    if (currentEvents.empty()) newDeltaTime += STEP_SIZE;
    for (const Event& cev : currentEvents) {
        track += deltaTime;
        deltaTime = 0;
        track += 128; // NOTE OFF
        track += static_cast<uint8_t>(cev.instr)+36; // KEY
        track += 100; // VEL
    }
    deltaTime = newDeltaTime;
    deltaTime = 0;
    
    // track += hexToByteString("00903C640490436404803C6400804364");

    track += deltaTime;
    track += hexToByteString("FF2F00"); // end of clip

    // Open and write to file
    std::ofstream f{p, std::ios::binary};
    
    f << "MThd";
    writeByteString(f, hexToByteString("0000000600000001004"));
    
    f << "MTrk";
    
    ByteString trackSize{0, 0, 0, static_cast<uint8_t>(track.size())}; // TODO: FIX THIS MESS OEGHG
    writeByteString(f, trackSize);
    writeByteString(f, track);

    return true;
}

std::string MIDISequence::to_string() const {
    std::string result = "MIDISequence:\n";
    for (const Event& ev : events) {
        result += "\tt: " + std::to_string(ev.t) + "\tinstr: " + std::to_string(static_cast<char>(ev.instr)) + "\n";
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
        uint8_t byte = std::stoi(hex.substr(i, 2), 0, 16);
        bs.push_back(byte);
    }
    return bs;
}

void writeByteString(std::ofstream& f, ByteString bs) {
    f.write(reinterpret_cast<const char*>(bs.data()), bs.size());
}