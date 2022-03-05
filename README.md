# Fuzz

Experimental repository to test Aseprite decoders with
[libfuzzer](http://llvm.org/docs/LibFuzzer.html) for the following
file format decoders:

* [aseprite](https://github.com/aseprite/aseprite/blob/main/src/dio/aseprite_decoder.cpp)
* [flic](https://github.com/aseprite/flic)
* [tga](https://github.com/aseprite/tga)

Note: We have used a private corpus of FLI/FLC, TGA, etc. files as
initial seed.

## fuzz_ase

To compile `fuzz_ase`, you will need a clone of
[`aseprite`](https://github.com/aseprite/aseprite) repository in the
same location of the `fuzz` project:

    +- root folder
    |
    `-+- aseprite
      |
      `- fuzz
