#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <vector>

enum class Instrument {kick = 36, snare = 37, hihat = 38, openhat = 39};

class MIDISequence {
public:

    typedef uint32_t MIDITick;
    typedef uint8_t Byte;
    typedef std::vector<MIDISequence::Byte> ByteString;

    struct Event {
        MIDITick t; // time in ticks
        Byte message;
        Instrument instr;
    };

    std::vector<Event> events;
    size_t num_steps = 16;

    MIDISequence();
    MIDISequence(size_t num_steps);
    MIDISequence(const MIDISequence&) = default;
    MIDISequence& operator=(const MIDISequence&) = default;
    MIDISequence(MIDISequence&&) = default;
    MIDISequence& operator=(MIDISequence&&) = default;

    void addEvent(MIDITick t, Byte message, Instrument instr);
    void addNote(MIDITick t, MIDITick len, Instrument instr);

    void append(const MIDISequence&);
    void sort();

    std::vector<Byte> writeToBuffer();
    bool writeToFile(std::string);

    std::string to_string() const;


    // HELPER FUNCTIONS
    static ByteString hexToByteString(const std::string& hex);
    static void writeByteString(std::ofstream& f, ByteString bs);
    static ByteString makeVariableLengthQuantity(MIDITick val);
};

std::ostream& operator<<(std::ostream&, const MIDISequence&);