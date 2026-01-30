#include <iostream>
#include <vector>

#include "midi.h"

MIDISequence::MIDISequence(): events{} {};

void MIDISequence::addEvent(char t, Instrument instr) {
    MIDISequence::Event event{t, instr};
    events.push_back(event);
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