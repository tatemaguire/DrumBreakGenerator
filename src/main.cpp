#include <iostream>
#include <string>
#include <random>

#include "generator.hpp"
#include "midi.hpp"

void print_help() {
    std::cout << "Usage: DrumBreakGenerator <num_steps> [options]" << std::endl
        << "                   num_steps is the number of 16th notes in the pattern" << std::endl
        << std::endl << "Options" << std::endl
        << "  -o <file path>" << std::endl
        << "                 = output file path [include the file extension .mid]" << std::endl
        << "  --kick <density> <sub_density>" << std::endl
        << "                 = kick density and sub_density parameters [0.0 - 1.0]" << std::endl
        << "  --snare <density> <sub_density>" << std::endl
        << "                 = snare density and sub_density parameters [0.0 - 1.0]" << std::endl
        << "  --hh <density> <sub_density>" << std::endl
        << "                 = closed hi-hat density and sub_density parameters [0.0 - 1.0]" << std::endl
        << "  --oh <density> <sub_density>" << std::endl
        << "                 = open hi-hat density and sub_density parameters [0.0 - 1.0]" << std::endl
        << "  --seed <seed_number>" << std::endl
        << "                 = generator seed" << std::endl;
}

void print_usage() {
    std::cout << "Usage: DrumBreakGenerator <num_steps> [options]" << std::endl
            << "\t\tTo see a list of options run:  DrumBreakGenerator --help" << std::endl;
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        print_usage();
        return 1;
    }

    // First argument should be --help or num_steps
    if (std::string(argv[1]) == "--help") {
        print_help();
        return 0;
    } 
    int num_steps = std::stoi(argv[1]);
    
    // values to be filled by arguments
    std::random_device rd; auto seed = rd();
    std::string out_path = "out.mid";
    std::vector<InstrumentConfig> configs;

    // Parse arguments
    for (int argi = 2; argi < argc; argi++) {

        int args_left = argc - 1 - argi;
        std::string argstr = argv[argi];

        if (argstr == "--seed") {
            if (args_left >= 1) {
                seed = std::stoull(argv[argi + 1]);
                argi++;
            }
            else {
                print_usage();
                return 1;
            }
        }
        else if (argstr == "-o") {
            if (args_left >= 1) {
                out_path = argv[argi + 1];
                argi++;
            }
            else {
                print_usage();
                return 1;
            }
        }
        else if (argstr == "--kick") {
            if (args_left >= 2) {
                InstrumentConfig c = {36, std::stof(argv[argi + 1]), std::stof(argv[argi + 2])};
                configs.emplace_back(c);
                argi += 2;
            }
            else {
                print_usage();
                return 1;
            }
        }
        else if (argstr == "--snare") {
            if (args_left >= 2) {
                InstrumentConfig c = {37, std::stof(argv[argi + 1]), std::stof(argv[argi + 2])};
                configs.emplace_back(c);
                argi += 2;
            }
            else {
                print_usage();
                return 1;
            }
        }
        else if (argstr == "--hh") {
            if (args_left >= 2) {
                InstrumentConfig c = {38, std::stof(argv[argi + 1]), std::stof(argv[argi + 2])};
                configs.emplace_back(c);
                argi += 2;
            }
            else {
                print_usage();
                return 1;
            }
        }
        else if (argstr == "--oh") {
            if (args_left >= 2) {
                InstrumentConfig c = {39, std::stof(argv[argi + 1]), std::stof(argv[argi + 2])};
                configs.emplace_back(c);
                argi += 2;
            }
            else {
                print_usage();
                return 1;
            }
        }
        else {
            print_usage();
            return 1;
        }
    }

    // generation
    DrumBreakGenerator generator = {seed};
    std::cout << "Seed: " << generator.seed << std::endl;
    MIDISequence seq = generator.generateSequence(num_steps, configs);
    
    // print results
    std::cout << seq << std::endl;

    // write file
    seq.writeToFile(out_path);

    return 0;
}