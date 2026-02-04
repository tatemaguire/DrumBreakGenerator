cmake_minimum_required(VERSION 3.31.2)

project(DrumBreakGenerator)

add_executable(DrumBreakGenerator)

target_sources(DrumBreakGenerator
    PRIVATE
        src/drumBreakGenerator.cpp
)

target_link_libraries(DrumBreakGenerator
    PRIVATE
        MIDISequence
)

add_executable(MIDITest)

target_sources(MIDITest
    PRIVATE
        test/midiTest.cpp
)

target_link_libraries(MIDITest
    PRIVATE
        MIDISequence
)

add_library(MIDISequence)

target_sources(MIDISequence
    PRIVATE
        src/midi.cpp
    PUBLIC
        src/midi.hpp
)