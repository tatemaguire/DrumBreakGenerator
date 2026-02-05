#pragma once

#include "midi.hpp"

MIDISequence generateSequence(float density, float subDensity, float chaos);
MIDISequence generateRhythm(Instrument instr, float density, float subDensity, float chaos);
int randInt(int min, int max);
