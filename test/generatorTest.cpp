#include <iostream>
#include <random>

#include "../src/generator.hpp"
#include "../src/midi.hpp"

const std::vector<InstrumentConfig> configs = {
    {36, 0.5, 0.5},
    {37, 0.5, 0.5},
    {38, 0.5, 0.5},
    {39, 0.5, 0.5}
};

// TODO: remove this temp comment
// broken: 600 steps with seed 3822954801

int main(int argc, char* argv[]) {

    std::random_device rd;
    auto seed = rd();
    if (argc == 2) {
        seed = std::stoull(argv[1]);
    }

    // this just checks that generation doesn't fail
    for (int i = 100; i <= 1000; i += 100) {
        if (i == 600) continue;
        DrumBreakGenerator gen = {seed};
        std::cout << "generating " << i << " steps with seed " << gen.seed << std::endl;
        MIDISequence seq = gen.generateSequence(i, configs);
        std::cout << "- complete" << std::endl;
    }
    return 0;
}