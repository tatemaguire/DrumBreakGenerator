#include <iostream>
#include <cmath>
#include <string>
#include <map>
#include <random>

#include "midi.hpp"
#include "generator.hpp"

DrumBreakGenerator::DrumBreakGenerator() {
    std::random_device rd;
    this->seed = rd();
    this->re.seed(this->seed);
}

DrumBreakGenerator::DrumBreakGenerator(std::default_random_engine::result_type seed):
    seed{seed}, re{seed} {};

// inclusive min and max
int DrumBreakGenerator::randInt(int min, int max) {
    std::uniform_int_distribution<int> dist{min, max};
    return dist(this->re);
}

// first int is the result, second int is the number of spots in the total pool
int DrumBreakGenerator::randIntWeighted(std::map<int, int> probabilities) {
    int total_pool = 0;
    for (auto entry : probabilities) {
        total_pool += entry.second;
    }

    if (total_pool == 0) {
        throw std::range_error("generator.cpp: randIntWeighted(): all probabilities are 0??");
    }

    int result = -1;
    
    int choice = randInt(0, total_pool-1);
    int current_pool = 0;

    for (auto entry : probabilities) {
        current_pool += entry.second;
        if (current_pool > choice) {
            result = entry.first;
            break;
        }
    }

    if (result == -1) {
        throw std::range_error("generator.cpp: randIntWeighted(): random result not found");
    }

    return result;
}

// min and max are inclusive
// param is 0 - 1, center is 0 - 1
// when param == center, you have the highest probability
// returns 0 - 1, resulting probability
float DrumBreakGenerator::parameterToProbability(float param, float min, float max, float center) {
    if (param >= min && param <= max) {
        return 1 - std::abs(center - param);
    } else {
        return 0;
    }
}

// generate one instrument's rhythm and put it in a MIDISequence
// external offset is based on other rhythms previously generated, it is also modified for the next rhythm
MIDISequence DrumBreakGenerator::generateRhythm(size_t num_steps, InstrumentConfig config, size_t& offset) {
    offset += randIntWeighted({{0, 1}, {1, 5}, {2, 2}, {3, 1}});
    offset %= 4; // loop offset from 0-3

    std::map<int, int> gap_probabilities = {};
    gap_probabilities[1] = parameterToProbability(config.density, 0.75, 1, 1) * 20;
    gap_probabilities[2] = parameterToProbability(config.density, 0.7, 1, 0.9) * 50;
    gap_probabilities[3] = parameterToProbability(config.density, 0.6, 1, 0.8) * 50;
    gap_probabilities[4] = parameterToProbability(config.density, 0.4, 0.7, 0.55) * 80;
    gap_probabilities[5] = parameterToProbability(config.density, 0.3, 0.5, 0.4) * 40;
    gap_probabilities[6] = parameterToProbability(config.density, 0.1, 0.4, 0.25) * 50;
    gap_probabilities[7] = parameterToProbability(config.density, 0.1, 0.3, 0.15) * 30;
    gap_probabilities[8] = parameterToProbability(config.density, 0.05, 0.2, 0.1) * 30;
    gap_probabilities[9] = parameterToProbability(config.density, 0, 0.2, 0.1) * 30;
    gap_probabilities[10] = parameterToProbability(config.density, 0, 0.2, 0.1) * 30;
    gap_probabilities[11] = parameterToProbability(config.density, 0, 0.2, 0) * 30;
    gap_probabilities[12] = parameterToProbability(config.density, 0, 0.1, 0) * 30;
    gap_probabilities[13] = parameterToProbability(config.density, 0, 0.1, 0) * 40;
    gap_probabilities[14] = parameterToProbability(config.density, 0, 0.1, 0) * 40;

    size_t gap = randIntWeighted(gap_probabilities);

    std::map<int, int> subdiv_probabilities = {};
    subdiv_probabilities[1] = parameterToProbability(config.sub_density, 0, 1, 0) * 100;
    subdiv_probabilities[2] = parameterToProbability(config.sub_density, 0, 1, 1) * 20;
    subdiv_probabilities[3] = parameterToProbability(config.sub_density, 0.5, 1, 1) * 5;

    MIDISequence rhythm = MIDISequence(num_steps);

    for (size_t step = offset; step < num_steps; step += gap) {
        if (randInt(1, 5) == 1) continue; // note skips 

        // generate subdivision
        int subdiv = randIntWeighted(subdiv_probabilities);
        int noteLen = rhythm.step_size / subdiv;
        for (int j = 0; j < rhythm.step_size; j += noteLen) {
            rhythm.addNote(step * rhythm.step_size + j, noteLen, config.instrument);
        }
    }

    return rhythm;
}

// generate the entire MIDISequence based on arguments
MIDISequence DrumBreakGenerator::generateSequence(size_t num_steps, std::vector<InstrumentConfig> configs) {
    // Check for domain errors
    if (num_steps <= 0) {
        throw std::domain_error("Generator: generateSequence(): num_steps must be > 0");
    }
    for (size_t i = 0; i < configs.size(); i++) {
        InstrumentConfig c = configs[i];
        if (c.density < 0 || c.density > 1) {
            throw std::domain_error("Generator: generateSequence(): density_" + std::to_string(i) + " must be > 0 and < 1");
        }
        if (c.sub_density < 0 || c.sub_density > 1) {
            throw std::domain_error("Generator: generateSequence(): subdensity_" + std::to_string(i) + " must be > 0 and < 1");
        }
    }

    size_t offset = randInt(0, 3);

    MIDISequence result{num_steps};

    for (const InstrumentConfig& c : configs) {
        MIDISequence rhythm = generateRhythm(num_steps, c, offset);
        result.append(rhythm);
    }

    if (result.events.size() == 0) result.addNote(0, 4, 36); // if empty, make a C3

    result.sort();

    return result;
}