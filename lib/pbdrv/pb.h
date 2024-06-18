#pragma once

#define PB_CLOCK_SPEED_HZ 100000
#define PB_TIMEOUT_MS 10
#define PB_TIMEOUT_US (1e3 * PB_TIMEOUT_MS)

// Adafruit NeoTrellis modules
#define PB_ADDR_ADA_NEO_1 0x2E
#define PB_ADDR_ADA_NEO_2 0x2F
#define PB_ADDR_ADA_NEO_3 0x30
#define PB_ADDR_ADA_NEO_4 0x32

// Main controller
#define PB_ADDR_MOD_MAIN 0x08

// Puzzle modules
#define PB_ADDR_MOD_NEOTRELLIS 0x21
#define PB_ADDR_MOD_SOFTWARE 0x22
#define PB_ADDR_MOD_HARDWARE 0x23
#define PB_ADDR_MOD_VAULT 0x24
// #define BUSADDR_MOD_AUTOMATION 0x25
#define PB_ADDR_MOD_DUMMY 0x69

