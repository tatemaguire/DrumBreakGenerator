#include <iostream>

#include "../src/midi.hpp"

int main(int argc, char* argv[]) {
    MIDISequence A{};

    A.addEvent(10, MIDISequence::Instrument::kick);
    A.addEvent(15, MIDISequence::Instrument::kick);
    A.addEvent(3, MIDISequence::Instrument::snare);
    A.addEvent(1, MIDISequence::Instrument::snare);
    A.addEvent(2, MIDISequence::Instrument::snare);
    for (int i = 0; i < 16; i++) {
        A.addEvent(i, MIDISequence::Instrument::hihat);
    }

    MIDISequence B{A};
    std::cout << "A: " << A << std::endl;
    std::cout << "B: " << B << std::endl;
    A.sort();
    std::cout << "A: " << A << std::endl;
    std::cout << "B: " << B << std::endl;

    A.writeToFile("out.mid");

    return 0;
}