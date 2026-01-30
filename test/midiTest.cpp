#include <iostream>

#include "../src/midi.hpp"

int main(int argc, char* argv[]) {
    MIDISequence A{};

    A.addEvent(10, MIDISequence::Instrument::kick);
    A.addEvent(3, MIDISequence::Instrument::hihat);
    A.addEvent(1, MIDISequence::Instrument::hihat);
    A.addEvent(2, MIDISequence::Instrument::hihat);

    MIDISequence B{A};
    std::cout << "A: " << A << std::endl;
    std::cout << "B: " << B << std::endl;
    A.sort();
    std::cout << "A: " << A << std::endl;
    std::cout << "B: " << B << std::endl;

    std::cout << __cplusplus << std::endl;

    return 0;
}