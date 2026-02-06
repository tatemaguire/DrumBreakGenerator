#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <vector>

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

    std::basic_string<unsigned char> writeToBuffer();
    bool writeToFile(std::string);

    std::string to_string() const;
};

// temporary, while seq_size is constant
const unsigned char get_seq_size();
std::ostream& operator<<(std::ostream&, const MIDISequence&);