#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

#include "midi.hpp"
#include "generator.hpp"

MIDISequence generateSequence(size_t num_steps, float density, float subDensity) {
    srand(time(0));

    if (density < 0.5) {
        density *= 2;
        density = std::pow(density, 0.5);
        density /= 2;
    }

    MIDISequence A = generateRhythm(num_steps, Instrument::kick, density, subDensity);
    MIDISequence B = generateRhythm(num_steps, Instrument::snare, density, subDensity);
    MIDISequence C = generateRhythm(num_steps, Instrument::hihat, density, subDensity);
    MIDISequence D = generateRhythm(num_steps, Instrument::openhat, density, subDensity);

    A.append(B);
    A.append(C);
    A.append(D);

    if (A.events.size() == 0) A.addNote(0, 4, Instrument::snare);

    A.sort();

    return A;
}

MIDISequence generateRhythm(size_t num_steps, Instrument instr, float density, float subDensity) {
    int division = std::ceil(randInt(1, 4) * density);
    division = 5 - division;
    int offset = randInt(0, division);

    int maxSubdiv = static_cast<int>(subDensity * 2 + 0.5);

    MIDISequence result{num_steps};

    for (int i = offset; i < num_steps; i += division) {
        float roll = randInt(1, 10) / 10.0;
        if (roll < density) {
            unsigned char subdiv = static_cast<unsigned char>(std::pow(2, randInt(0, randInt(0, maxSubdiv))));
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