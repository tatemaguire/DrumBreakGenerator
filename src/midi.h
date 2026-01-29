#pragma once

#include <iostream>
#include <string>

class MIDISequence {
public:
    enum class Instrument {kick, snare, hihat, openhat};

    struct Event {
        char t; // step value 0-15
        Instrument instr;
    };

    std::vector<Event> events;

    MIDISequence();
    MIDISequence(const MIDISequence&) = default;
    MIDISequence& operator=(const MIDISequence&) = default;
    MIDISequence(MIDISequence&&) = default;
    MIDISequence& operator=(MIDISequence&&) = default;

    void addEvent(char t, Instrument instr);

    std::string to_string() const;
};

std::ostream& operator<<(std::ostream&, const MIDISequence&);