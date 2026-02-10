#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <vector>

enum class Instrument {kick, snare, hihat, openhat};

class MIDISequence {
public:
    struct Event {
        char t; // time in ticks
        unsigned char message;
        Instrument instr;
    };

    std::vector<Event> events;

    MIDISequence();
    MIDISequence(const MIDISequence&) = default;
    MIDISequence& operator=(const MIDISequence&) = default;
    MIDISequence(MIDISequence&&) = default;
    MIDISequence& operator=(MIDISequence&&) = default;

    void addEvent(char t, unsigned char message, Instrument instr);
    void addNote(char t, char len, Instrument instr);

    void append(const MIDISequence&);
    void sort();

    std::vector<unsigned char> writeToBuffer();
    bool writeToFile(std::string);

    std::string to_string() const;
};

// temporary, while seq_size is constant
const unsigned char get_seq_size();
std::ostream& operator<<(std::ostream&, const MIDISequence&);