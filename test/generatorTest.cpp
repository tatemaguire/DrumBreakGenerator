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

// creates 4 instrument configs with density and sub_density
std::vector<InstrumentConfig> debugConfigs(float density, float sub_density) {
    std::vector<InstrumentConfig> configs = {};
    
    for (int i = 0; i < 4; i++) {
        Instrument instr;
        switch (i)
        {
            case 0:
                instr = Instrument::kick;
                break;
            case 1:
                instr = Instrument::snare;
                break;
            case 2: 
                instr = Instrument::hihat;
                break;
            case 3:
                instr = Instrument::openhat;
                break;
            default:
                throw std::domain_error("Instrument" + std::to_string((i-2)/2) + " is not defined");
                break;
        }
        InstrumentConfig c = {instr, density, sub_density};
        configs.push_back(c);
    }

    return configs;
}

int makeBatch(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <num steps>" << std::endl;
        return 1;
    }

    int num_steps = std::stoi(argv[1]);

    std::string out_name = "out_";
    std::string file_extension = ".mid";
    int file_ID = 0;

    DrumBreakGenerator generator = {};

    for (int i = 0; i <= 10; i++) {
        float density = 0.1 * i;

        if (std::abs(density - 1) < 0.05) density = 1;
        std::cout << density << std::endl;
        MIDISequence seq = generator.generateSequence(num_steps, debugConfigs(density, 0));
        seq.writeToFile(out_name + std::to_string(file_ID++) + file_extension);
    }

    return 0;
}

int main(int argc, char* argv[]) {

    // return makeBatch(argc, argv);

    if (argc < 4) {
        std::cout << "Usage: " << argv[0] << " <num steps> [<density_1> <subdensity_1>] [<density_2> <subdensity_2>] ..." << std::endl;
        return 1;
    }

    // get values from arguments
    int num_steps = std::stoi(argv[1]);
    std::vector<InstrumentConfig> configs = parseArgsToConfigs(argc, argv);

    // generation
    DrumBreakGenerator generator = {1886653960};
    std::cout << "Seed: " << generator.seed << std::endl;
    MIDISequence seq = generator.generateSequence(num_steps, configs);
    
    // print results
    std::cout << seq << std::endl;

    // write file
    seq.writeToFile(out_path);

    return 0;
}