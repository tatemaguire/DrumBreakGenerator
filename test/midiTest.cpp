#include <iostream>

#include "../src/midi.h"

int main(int argc, char* argv[]) {
    MIDISequence A{};

    A.addEvent(10, MIDISequence::Instrument::kick);
    A.addEvent(3, MIDISequence::Instrument::hihat);
    A.addEvent(1, MIDISequence::Instrument::hihat);
    A.addEvent(2, MIDISequence::Instrument::hihat);

    std::cout << A << std::endl;
    A.sort();
    std::cout << A << std::endl;

    return 0;
}