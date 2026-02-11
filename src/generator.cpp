#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

#include "midi.hpp"
#include "generator.hpp"

MIDISequence generateSequence(size_t num_steps, std::vector<InstrumentConfig> configs) {
    srand(time(0));

    MIDISequence A = generateRhythm(num_steps, configs[0]);
    MIDISequence B = generateRhythm(num_steps, configs[1]);
    MIDISequence C = generateRhythm(num_steps, configs[2]);
    MIDISequence D = generateRhythm(num_steps, configs[3]);

    A.append(B);
    A.append(C);
    A.append(D);

    if (A.events.size() == 0) A.addNote(0, 4, Instrument::snare);

    A.sort();

    return A;
}

MIDISequence generateRhythm(size_t num_steps, InstrumentConfig config) {
    Instrument instr = config.instrument;
    float density = config.density;
    float sub_density = config.sub_density;

    if (density < 0.5) {
        density *= 2;
        density = std::pow(density, 0.5);
        density /= 2;
    }

    int division = std::ceil(randInt(1, 4) * density);
    division = 5 - division;
    int offset = randInt(0, division);

    int max_subdiv = static_cast<int>(sub_density * 2 + 0.5);

    MIDISequence result{num_steps};

    for (int i = offset; i < num_steps; i += division) {
        float roll = randInt(1, 10) / 10.0;
        if (roll < density) {
            unsigned char subdiv = static_cast<unsigned char>(std::pow(2, randInt(0, randInt(0, max_subdiv))));
            unsigned char noteLen = 4 / subdiv;
            for (int j = 0; j < 4; j += noteLen) {
                result.addNote(i * 4 + j, noteLen, instr);
            }
        }
    }

    return result;
}

// inclusive min and max
int randInt(int min, int max) {
    max++;
    int num = rand() % (max-min);
    return num + min;
}