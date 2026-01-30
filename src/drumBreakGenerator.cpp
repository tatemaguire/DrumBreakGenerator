#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "midi.hpp"

MIDISequence generateRhythm(float density, MIDISequence::Instrument instr);
int randInt(int min, int max);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <density 0-1> <filepath>" << std::endl;
        return 1;
    }

    srand(time(0));

    float density = std::stof(argv[1]);
    
    // generation
    MIDISequence A = generateRhythm(density, MIDISequence::Instrument::kick);
    MIDISequence B = generateRhythm(density, MIDISequence::Instrument::snare);
    MIDISequence C = generateRhythm(density, MIDISequence::Instrument::hihat);
    MIDISequence D = generateRhythm(density, MIDISequence::Instrument::openhat);

    A.append(B);
    A.append(C);
    A.append(D);

    if (A.events.size() == 0) A.addEvent(0, MIDISequence::Instrument::snare);

    A.sort();
    std::cout << A << std::endl;


    // write file
    A.writeToFile(argv[2]);

    return 0;
}

MIDISequence generateRhythm(float density, MIDISequence::Instrument instr) {
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