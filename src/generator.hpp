#pragma once

#include "midi.hpp"

MIDISequence generateSequence(float density);
MIDISequence generateRhythm(float density, Instrument instr);
int randInt(int min, int max);
