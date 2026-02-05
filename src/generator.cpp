#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

#include "midi.hpp"
#include "generator.hpp"

MIDISequence generateSequence(float density) {
    MIDISequence A = generateRhythm(density, Instrument::kick);
    MIDISequence B = generateRhythm(density, Instrument::snare);
    MIDISequence C = generateRhythm(density, Instrument::hihat);
    MIDISequence D = generateRhythm(density, Instrument::openhat);

    A.append(B);
    A.append(C);
    A.append(D);

    if (A.events.size() == 0) A.addEvent(0, Instrument::snare);

    A.sort();

    return A;
}

MIDISequence generateRhythm(float density, Instrument instr) {
    int division = std::ceil(randInt(1, 4) * density);
    division = 5 - division;
    int offset = randInt(0, division);

    MIDISequence result{};

    for (int i = offset; i < SEQ_SIZE; i += division) {
        float roll = randInt(1, 10) / 10.0;
        if (roll < density) result.addEvent(i, instr);
    }

    return result;
}

// inclusive min and max
int randInt(int min, int max) {
    max++;
    int num = rand() % (max-min);
    return num + min;
}