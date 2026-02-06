#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

#include "midi.hpp"
#include "generator.hpp"

MIDISequence generateSequence(float density, float subDensity, float chaos) {
    srand(time(0));

    MIDISequence A = generateRhythm(Instrument::kick, density, subDensity, chaos);
    MIDISequence B = generateRhythm(Instrument::snare, density, subDensity, chaos);
    MIDISequence C = generateRhythm(Instrument::hihat, density, subDensity, chaos);
    MIDISequence D = generateRhythm(Instrument::openhat, density, subDensity, chaos);

    A.append(B);
    A.append(C);
    A.append(D);

    if (A.events.size() == 0) A.addNote(0, 4, Instrument::snare);

    A.sort();

    return A;
}

MIDISequence generateRhythm(Instrument instr, float density, float subDensity, float chaos) {
    int division = std::ceil(randInt(1, 4) * density);
    division = 5 - division;
    int offset = randInt(0, division);

    int maxSubdiv = static_cast<int>(subDensity * 2 + 0.5);

    MIDISequence result{};

    for (int i = offset; i < get_seq_size(); i += division) {
        float roll = randInt(1, 10) / 10.0;
        if (roll < density) {
            unsigned char subdiv = static_cast<unsigned char>(std::pow(2, randInt(0, randInt(0, maxSubdiv))));
            unsigned char noteLen = 4 / subdiv;
            std::cout << std::to_string(subdiv) << " " << std::to_string(noteLen) << std::endl;
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