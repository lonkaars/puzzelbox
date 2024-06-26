\defgroup main main
\brief Main controller software

This directory contains the software for the main controller of the Puzzle Box.
The main controller target is a Raspberry Pi Pico W.

## Building

1. Create a `config.h` file (see [config](#main_config))
2. Use CMake to build

## Flashing

1. Hold the BOOTSEL button while resetting the Pico (by power cycling or
   pulling pin 30 (RUN) to GND)
2. ```
   $ picotool load build/main.uf2
   ```

