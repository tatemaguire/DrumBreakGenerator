cmake_minimum_required(VERSION 4.2.1)

project(DrumBreakGenerator)

add_executable(DrumBreakGenerator)

target_sources(DrumBreakGenerator
    PRIVATE
        src/drumBreakGenerator.cpp
)