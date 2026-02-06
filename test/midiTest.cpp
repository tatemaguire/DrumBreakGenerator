#include <iostream>

#include "../src/midi.hpp"

int main(int argc, char* argv[]) {
    MIDISequence A{};

    const char step = 4;

    A.addNote(10*step, step, Instrument::kick);
    A.addNote(15*step, step, Instrument::kick);
    A.addNote(3*step, step, Instrument::snare);
    A.addNote(1*step, step, Instrument::snare);
    A.addNote(2*step, step, Instrument::snare);
    for (int i = 0; i < 16; i++) {
        A.addNote(i*step, step, Instrument::hihat);
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