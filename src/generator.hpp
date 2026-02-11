#pragma once

#include "midi.hpp"

struct InstrumentConfig {
    Instrument instrument;
    float density;
    float sub_density;
};

MIDISequence generateSequence(size_t num_steps, std::vector<InstrumentConfig> config);
