# Drum Break Generator
By Tate Maguire

For CMPM 147 at UCSC

This Drum Break Generator randomly generates a drum pattern according to the parameters set by the user.
This pattern is then exported as a MIDI file to be used in a DAW (Digital Audio Workstation), where it 
can be played back using drum sounds of your choosing.
This tool is meant to be useful for music producers to ideate rhythms, but could also
be implemented in real-time applications like video games to randomly generate percussion.

# Two ways to use

## 1. Website (Easy)
You can easily generate and download midi files using the website:
[https://tatemaguire.github.io/DrumBreakGeneratorWebsite/](https://tatemaguire.github.io/DrumBreakGeneratorWebsite/)

## 2. Building the Command Line Interface (Difficult)

### Compiling
After cloning this repository, in the root folder you will compile using CMake. Make sure you have CMake installed.

```sh
cmake -B build
cmake --build build
```

This will create the `build` folder with the proper settings for building, and then build the tests and CLI inside that folder.

### Running the Generator
```sh
# Linux/MacOS
build/DrumBreakGenerator

# Windows
build/DrumBreakGenerator.exe
```
Use `--help` to learn how to set parameters:
```sh
build/DrumBreakGenerator --help
```

# Parameters
## Pattern Parameters
**Number of Steps**: the number of 16th notes that will be generated.

**Seed**: used to seed the random number generator. Leave this blank for a completely random pattern.
To recreate a previous generation, use its seed (which is printed out with the rest of the pattern data).
You can also extend the length of a previous generation by using the seed and increasing the Number of Steps.

**Output Filepath**: when using this tool from the command line, you can specify the name of the output file using the `-o` option.
This file should ideally end with the file extension `.mid` to specify that it's a MIDI file.

## Instrument Parameters
These parameters affect one instrument layer in the pattern. 
The instruments are: Kick, Snare, Closed Hi-Hat, and Open Hi-hat.
Both of these values should be entered as a number between 0 and 1.

**Density**: how often that instrument is played in the pattern.

**Subdivision Density**: how often those notes are split into two 32nd notes or four 64th notes. 

# Example Outputs
See example outputs:
[https://youtu.be/Z_Yj49y-9wA](https://youtu.be/Z_Yj49y-9wA)

# Known Limitations
The tool currently lacks a consistency in the way it generates patterns. Typically when someone creates a rhythm,
they will keep in mind how different percussion instruments work together. However, this generator 
(except for a small offset), generates each instrument's rhythm independently of one another.

It also makes rhythms on a quantized 16th note grid, so a lot of interesting rhythms are impossible to generate.
I tried to get around this using the subdivision density, but it's not quite the same.