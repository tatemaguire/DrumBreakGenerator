# Drum Break Generator
By Tate Maguire

For CMPM 147 at UCSC

# How To Use
There are 2 ways to interface with this library. You can easily generate and download midi files using the website, or clone this repo and implement the c++ library into your own project.

## 1. Website
For an easy way to generate and download drum patterns as MIDI files, access the website [here!](https://tatemaguire.github.io/DrumBreakGeneratorWebsite/)

## 2. Command Line Interface
To use the CLI, you will have to build the binary application yourself, using cmake.

### Compiling
------------------------
After cloning this repository, in the root folder you will compile using CMake. Make sure you have CMake installed.

```sh
cmake -B build
cmake --build build
```

This will create the `build` folder with the proper settings for building, and then build the tests and CLI inside that folder.

### Running the Generator
------------------------

### Running Tests
------------------------
On MacOS or Linux:
```sh
build/GeneratorTest

build/MIDITest
```

On Windows (I might be wrong here):
```sh
build/GeneratorTest.exe

build/MIDITest.exe
```
MIDI file will by default be written to `out.mid` in the working directory.
