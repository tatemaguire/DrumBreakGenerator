#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <vector>

const char SEQ_SIZE = 16;

enum class Instrument {kick, snare, hihat, openhat};
extern std::vector<std::string> INSTRUMENT_NAMES;

class MIDISequence {
public:
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

    void append(const MIDISequence&);
    void sort();

    std::string writeToBuffer();
    bool writeToFile(std::string);

    std::string to_string() const;
};

std::ostream& operator<<(std::ostream&, const MIDISequence&);