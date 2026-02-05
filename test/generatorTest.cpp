#include <iostream>
#include "../src/generator.hpp"
#include "../src/midi.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <density 0-1>" << std::endl;
        return 1;
    }

    std::string outPath;
    if (argc >= 3) {
        outPath = argv[2];
    } else {
        outPath = "out.mid";
    }

    srand(time(0));

    float density = std::stof(argv[1]);
    
    // generation
    MIDISequence seq = generateSequence(density);
    
    std::cout << seq << std::endl;

    // write file
    seq.writeToFile(outPath);

    return 0;
}