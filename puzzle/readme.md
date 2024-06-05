# puzzles

This folder contains the source code for all puzzle modules.

## Arduino-based puzzle modules

Because of the poorly designed hardware (21-22) used during development
(23-24), some puzzle modules ended up being developed using Arduino boards. All
libraries in this repository use CMake for building (for consistency), which
also means the Arduino based puzzle modules use CMake. The CMakeLists.txt of
some puzzles uses the [Arduino-CMake-Toolchain][arduino-cmake]. To build any of
these subfolders, make sure you have done the following:

- Install the official Arduino IDE
- Open "Tools" > "Board" > "Board manager"
- Install the "Arduino AVR Boards" package (1.8.6 works at the time of writing)

[arduino-cmake]: https://github.com/a9183756-gh/Arduino-CMake-Toolchain
