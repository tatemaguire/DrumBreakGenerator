#include <iostream>
#include "../src/generator.hpp"
#include "../src/midi.hpp"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <density> <subdensity> <chaos>" << std::endl;
        return 1;
    }

    float density = std::stof(argv[1]);
    float subDensity = std::stof(argv[2]);
    float chaos = 0; // TODO: implement chaos


    std::string outPath;
    if (argc > 4) {
        outPath = argv[4];
    } else {
        outPath = "out.mid";
    }

    srand(time(0));
    
    // generation
    MIDISequence seq = generateSequence(density, subDensity, chaos);
    
    std::cout << seq << std::endl;

    // write file
    seq.writeToFile(outPath);

    return 0;
}