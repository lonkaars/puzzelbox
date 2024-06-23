\defgroup puz puzzle
\brief Puzzle modules

# puzzles

This folder contains the source code for all puzzle modules.

## Arduino-based puzzle modules

> [!NOTE]
> Because of the poorly designed hardware (21-22) used during development
> (23-24), all puzzle modules ended up being developed using Arduino boards.

All libraries in this repository use CMake for building (for consistency),
which also means the Arduino based puzzle modules use CMake. The CMakeLists.txt
of some puzzles uses the [Arduino-CMake-Toolchain][arduino-cmake]. To build any
of these subfolders, make sure you have done the following:

- Install the official Arduino IDE
- Open "Tools" > "Board" > "Board manager"
- Install the "Arduino AVR Boards" package (1.8.6 works at the time of writing)
- Install the Adafruit seesaw library and dependicies (for the neotrellis puzzle)
- Install the TM1637 library from Avishay Orpaz

[arduino-cmake]: https://github.com/a9183756-gh/Arduino-CMake-Toolchain

## ESP-based puzzle modules

### ESP-IDF SDK Setup instructions

1. Install ESP-IDF extension in Visual Studio Code
2. Install using 'express' option
3. Install ESP-IDF v5.2.1 (release version)

   Additional help:
   - [For windows](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/windows-setup.html#get-started-windows-first-steps)
   - [For Linux](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/linux-macos-setup.html#get-started-linux-macos-first-steps)

