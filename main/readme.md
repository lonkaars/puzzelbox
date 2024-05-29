# main controller firmware

This directory contains the software for the main controller of the Puzzle Box.

## building

1. make sure the submodules are initialized
2. copy [`config.def.h`](./config.def.h) to `config.h` and edit the defaults
3. `mkdir build`
4. `cmake -B build`
5. `make -C build` or `ninja -C build` (choose your preference)

alternatively, a makefile is provided for convenience

## "flashing"

1. [build](#building)
2. (re)connect the raspberry pi pico while holding the BOOTSEL button (this is
   the only button)
3. `picotool load build/main.uf2`

