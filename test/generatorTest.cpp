#include <iostream>
#include "../src/generator.hpp"
#include "../src/midi.hpp"

const std::string out_path = "out.mid";

std::vector<InstrumentConfig> parseArgsToConfigs(int argc, char* argv[]) {
    // Get configs from command line arguments
    std::vector<InstrumentConfig> configs = {};

    for (int i = 2; i < argc; i += 2) {
        // choose instrument based on argument number in this order: kick, snare, hihat, openhat
        Instrument instr;
        switch (i)
        {
            case 2:
                instr = Instrument::kick;
                break;
            case 4:
                instr = Instrument::snare;
                break;
            case 6: 
                instr = Instrument::hihat;
                break;
            case 8:
                instr = Instrument::openhat;
                break;
            default:
                throw std::domain_error("Instrument" + std::to_string((i-2)/2) + " is not defined");
                break;
        }
        float density = std::stof(argv[i]);
        float sub_density = std::stof(argv[i+1]);

        InstrumentConfig c = {instr, density, sub_density};
        configs.push_back(c);
    }

    return configs;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "Usage: " << argv[0] << " <num steps> [<density_1> <subdensity_1>] [<density_2> <subdensity_2>] ..." << std::endl;
        return 1;
    }

    // get values from arguments
    float num_steps = std::stoi(argv[1]);
    std::vector<InstrumentConfig> configs = parseArgsToConfigs(argc, argv);
    
    // generation
    MIDISequence seq = generateSequence(num_steps, configs);
    
    // print results
    std::cout << seq << std::endl;

    // write file
    seq.writeToFile(out_path);

    return 0;
}