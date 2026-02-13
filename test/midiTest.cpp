#include <iostream>

#include "../src/midi.hpp"

int main(int argc, char* argv[]) {
    MIDISequence A{};

    const char step = 4;

    A.addNote(10*step, step, 36);
    A.addNote(15*step, step, 36);
    A.addNote(3*step, step, 37);
    A.addNote(1*step, step, 37);
    A.addNote(2*step, step, 37);
    for (int i = 0; i < 16; i++) {
        A.addNote(i*step, step, 38);
    }

    MIDISequence B{A};
    std::cout << "A: " << A << std::endl;
    std::cout << "B: " << B << std::endl;
    A.sort();
    std::cout << "A: " << A << std::endl;
    std::cout << "B: " << B << std::endl;

    A.writeToFile("out.mid");

    // VLQ test
    MIDISequence::ByteString fourndred = MIDISequence::makeVariableLengthQuantity(40090);

    for (int i = 0; i < fourndred.size(); i++) {
        std::cout << static_cast<int>(fourndred[i]) << " "; // should be 2 57 26
    }

    return 0;
}