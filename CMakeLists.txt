cmake_minimum_required(VERSION 4.2.1)

project(DrumBreakGenerator)

add_executable(DrumBreakGenerator)
target_compile_features(DrumBreakGenerator PUBLIC cxx_std_20)

target_sources(DrumBreakGenerator
    PRIVATE
        src/drumBreakGenerator.cpp
)

add_executable(MIDITest)
target_compile_features(MIDITest PUBLIC cxx_std_20)

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
        src/midi.h
)