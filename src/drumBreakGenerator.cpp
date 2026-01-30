#include <iostream>

#include "midi.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <filepath>" << std::endl;
        return 1;
    }

    // generation
    MIDISequence A{};

    A.addEvent(10, MIDISequence::Instrument::kick);
    A.addEvent(3, MIDISequence::Instrument::hihat);
    A.addEvent(1, MIDISequence::Instrument::hihat);
    A.addEvent(2, MIDISequence::Instrument::hihat);

    // write file
    A.writeToFile(argv[1]);

    return 0;
}