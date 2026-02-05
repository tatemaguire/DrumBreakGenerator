cmake_minimum_required(VERSION 3.31.2)

project(DrumBreakGenerator)

# Executables

add_executable(GeneratorTest)
target_sources(GeneratorTest
    PRIVATE
        test/generatorTest.cpp
)
target_link_libraries(GeneratorTest
    PRIVATE
        MIDISequence
        Generator
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

# Libraries

add_library(MIDISequence)
target_sources(MIDISequence
    PRIVATE
        src/midi.cpp
    PUBLIC
        src/midi.hpp
)


add_library(Generator)
target_sources(Generator
    PRIVATE
        src/generator.cpp
    PUBLIC
        src/generator.hpp
)