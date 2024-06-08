# main controller firmware

This directory contains the software for the main controller of the Puzzle Box.

## building

1. make sure the submodules are initialized
2. create a `config.h` file and define some options (see `config.def.h` for all
   options):
   ```c
   #pragma once

   #define CFG_NET_SSID "network name"
   #define CFG_NET_PASS "network password"
   #define CFG_NET_AUTH CYW43_AUTH_WPA2_AES_PSK

   #include "config.def.h"
   ```
3. use CMake to build

## flashing

1. build
2. hold the BOOTSEL button while resetting the pico (by power cycling or
   pulling pin 30 (RUN) to GND)
3. `picotool load build/main.uf2`

