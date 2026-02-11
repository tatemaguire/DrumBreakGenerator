#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <map>

#include "midi.hpp"
#include "generator.hpp"

// inclusive min and max
int randInt(int min, int max) {
    max++;
    int num = rand() % (max-min);
    return num + min;
}

// first int is the result, second int is the number of spots in the total pool
int randIntWeighted(std::map<int, int> probabilities) {
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

// param is 0 - 1, center is 0 - 1
// when param == center, you have the highest probability
// returns 0 - 1, resulting probability
float probFromCenter(float param, float center) {
    return 1 - std::abs(center - param);
}

// TODO: min and max inclusive or exclusive?
float parameterToProbability(float param, float min, float max, float center) {
    if (param >= min && param <= max) {
        return probFromCenter(param, center);
    } else {
        return 0;
    }
}

// generate one instrument's rhythm and put it in a MIDISequence
// external offset is based on other rhythms previously generated, it is also modified for the next rhythm
MIDISequence generateRhythm(size_t num_steps, InstrumentConfig config, size_t& offset) {
    offset += randIntWeighted({{0, 1}, {1, 5}, {2, 2}, {3, 1}});
    offset %= 4; // loop offset from 0-3

    std::map<int, int> gap_probabilities = {};
    gap_probabilities[1] = parameterToProbability(config.density, 0.7, 1, 1) * 30;
    gap_probabilities[2] = parameterToProbability(config.density, 0.5, 1, 0.7) * 50;
    gap_probabilities[3] = parameterToProbability(config.density, 0.5, 0.8, 0.6) * 50;
    gap_probabilities[4] = parameterToProbability(config.density, 0.2, 0.5, 0.35) * 60;
    gap_probabilities[5] = parameterToProbability(config.density, 0.1, 0.4, 0.25) * 40;
    gap_probabilities[6] = parameterToProbability(config.density, 0.1, 0.3, 0.15) * 30;
    gap_probabilities[7] = parameterToProbability(config.density, 0.1, 0.2, 0.1) * 30;
    gap_probabilities[8] = parameterToProbability(config.density, 0.1, 0.2, 0) * 30;
    gap_probabilities[9] = parameterToProbability(config.density, 0.1, 0.2, 0) * 30;
    gap_probabilities[10] = parameterToProbability(config.density, 0.1, 0.2, 0) * 30;
    gap_probabilities[11] = parameterToProbability(config.density, 0, 0.2, 0) * 30;
    gap_probabilities[12] = parameterToProbability(config.density, 0, 0.1, 0) * 30;
    gap_probabilities[13] = parameterToProbability(config.density, 0, 0.1, 0) * 30;
    gap_probabilities[14] = parameterToProbability(config.density, 0, 0.1, 0) * 30;


    size_t gap = randIntWeighted(gap_probabilities);

    MIDISequence rhythm = MIDISequence(num_steps);

    for (size_t step = offset; step < num_steps; step += gap) {
        rhythm.addNote(step * rhythm.step_size, rhythm.step_size, config.instrument);
    }

    return rhythm;
}

// generate the entire MIDISequence based on arguments
MIDISequence generateSequence(size_t num_steps, std::vector<InstrumentConfig> configs) {
    // Check for domain errors
    if (num_steps <= 0) {
        throw std::domain_error("Generator: generateSequence(): num_steps must be > 0");
    }
    for (size_t i = 0; i < configs.size(); i++) {
        InstrumentConfig c = configs[i];
        if (c.density < 0 || c.density > 1) {
            throw std::domain_error("Generator: generateSequence(): density_" + std::to_string(i) + " must be > 0");
        }
        if (c.sub_density < 0 || c.sub_density > 1) {
            throw std::domain_error("Generator: generateSequence(): subdensity_" + std::to_string(i) + " must be > 0");
        }
    }

    // Seed generation
    srand(time(0));

    size_t offset = randInt(0, 3);

    MIDISequence A = generateRhythm(num_steps, configs[0], offset);
    MIDISequence C = generateRhythm(num_steps, configs[2], offset);
    MIDISequence B = generateRhythm(num_steps, configs[1], offset);
    MIDISequence D = generateRhythm(num_steps, configs[3], offset);

    A.append(B);
    A.append(C);
    A.append(D);

    if (A.events.size() == 0) A.addNote(0, 4, Instrument::kick);

    A.sort();

    return A;
}