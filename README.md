# Drum Break Generator
By Tate Maguire

For CMPM 147 at UCSC

## Compiling
Make sure you have CMake installed

```sh
cmake -B build
cmake --build build
```

## Running Tests on MacOS/Linux
```sh
build/GeneratorTest

build/MIDITest
```
MIDI file will by default be written to `out.mid` in the working directory.