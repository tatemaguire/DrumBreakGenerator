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

    MIDISequence result{};

    for (int i = offset; i < get_seq_size(); i += division) {
        float roll = randInt(1, 10) / 10.0;
        if (roll < density) {
            result.addNote(i*4, 4, instr);
            // float subdiv = std::pow(2, randInt(0, 2));
            // for (int j = 0; j < subdiv; j++) {
            //     result.addEvent(i, instr);
            // }
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