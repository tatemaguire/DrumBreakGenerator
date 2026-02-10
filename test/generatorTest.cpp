#include <iostream>
#include "../src/generator.hpp"
#include "../src/midi.hpp"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "Usage: " << argv[0] << " <num steps> <density> <subdensity>" << std::endl;
        return 1;
    }

    float num_steps = std::stoi(argv[1]);
    float density = std::stof(argv[2]);
    float subDensity = std::stof(argv[3]);


    std::string outPath;
    if (argc > 4) {
        outPath = argv[4];
    } else {
        outPath = "out.mid";
    }

    srand(time(0));
    
    // generation
    MIDISequence seq = generateSequence(num_steps, density, subDensity);
    
    std::cout << seq << std::endl;

    // write file
    seq.writeToFile(outPath);

    return 0;
}