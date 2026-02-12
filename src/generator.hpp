#pragma once

#include <random>
#include <map>

#include "midi.hpp"

struct InstrumentConfig {
    Instrument instrument;
    float density;
    float sub_density;
};

class DrumBreakGenerator {
private:
    std::default_random_engine re;

    int randInt(int min, int max);
    int randIntWeighted(std::map<int, int> probabilities);
    float parameterToProbability(float param, float min, float max, float center);
    MIDISequence generateRhythm(size_t num_steps, InstrumentConfig config, size_t& offset);
public:
    std::default_random_engine::result_type seed;

    DrumBreakGenerator();
    DrumBreakGenerator(std::default_random_engine::result_type seed);
    MIDISequence generateSequence(size_t num_steps, std::vector<InstrumentConfig> configs);
};
