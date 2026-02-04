#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "midi.hpp"

MIDISequence generateRhythm(float density, Instrument instr);
int randInt(int min, int max);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <density 0-1>" << std::endl;
        return 1;
    }

    std::string outPath;
    if (argc >= 3) {
        outPath = argv[2];
    } else {
        outPath = "out.mid";
    }

    srand(time(0));

    float density = std::stof(argv[1]);
    
    // generation
    MIDISequence seq = generateSequence(density);
    
    std::cout << seq << std::endl;

    // write file
    seq.writeToFile(outPath);

    return 0;
}

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