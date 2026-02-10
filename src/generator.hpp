#pragma once

#include "midi.hpp"

MIDISequence generateSequence(size_t num_steps, float density, float subDensity);
MIDISequence generateRhythm(size_t num_steps, Instrument instr, float density, float subDensity);
int randInt(int min, int max);
