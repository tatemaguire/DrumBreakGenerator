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
        result += "\tstep: " + std::to_string(ev.t) + "\tinstrument: " + std::to_string(static_cast<char>(ev.instr)) + "\n";
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const MIDISequence& seq) {
    return os << seq.to_string();
}