\defgroup main main
\brief Main controller software

# main controller firmware

This directory contains the software for the main controller of the Puzzle Box.

## building

1. make sure the submodules are initialized
2. create a `config.h` file (see \ref main_config "config")
3. use CMake to build

## flashing

1. build
2. hold the BOOTSEL button while resetting the pico (by power cycling or
   pulling pin 30 (RUN) to GND)
3. `picotool load build/main.uf2`

