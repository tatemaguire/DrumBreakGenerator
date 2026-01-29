#include <iostream>

#include "../src/midi.h"

int main(int argc, char* argv[]) {
    MIDISequence A{};

    A.addEvent(0, MIDISequence::Instrument::kick);
    A.addEvent(1, MIDISequence::Instrument::hihat);

    std::cout << A << std::endl;
    
    return 0;
}