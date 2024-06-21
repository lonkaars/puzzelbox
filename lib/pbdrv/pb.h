#pragma once

/**
 * \ingroup pbdrv
 * \ingroup pbdrv-mod
 * \defgroup pb Bus
 * \brief Constants for the puzzle bus hardware
 * \{
 */

//! I2C bus speed in hertz (100 KHz)
#define PB_CLOCK_SPEED_HZ 100000
//! I2C bus timeout delay in milliseconds
#define PB_TIMEOUT_MS 10
//! I2C bus timeout delay in microseconds
#define PB_TIMEOUT_US (1e3 * PB_TIMEOUT_MS)

//! Adafruit NeoTrellis module 1 I2C address
#define PB_ADDR_ADA_NEO_1 0x2E
//! Adafruit NeoTrellis module 2 I2C address
#define PB_ADDR_ADA_NEO_2 0x2F
//! Adafruit NeoTrellis module 3 I2C address
#define PB_ADDR_ADA_NEO_3 0x30
//! Adafruit NeoTrellis module 4 I2C address
#define PB_ADDR_ADA_NEO_4 0x32

//! Main controller I2C address
#define PB_ADDR_MOD_MAIN 0x08

//! NeoTrellis puzzle module I2C address
#define PB_ADDR_MOD_NEOTRELLIS 0x21
//! Software puzzle module I2C address
#define PB_ADDR_MOD_SOFTWARE 0x22
//! Hardware puzzle module I2C address
#define PB_ADDR_MOD_HARDWARE 0x23
//! Vault puzzle module I2C address
#define PB_ADDR_MOD_VAULT 0x24
//! Automation puzzle module I2C address
#define BUSADDR_MOD_AUTOMATION 0x25
//! Dummy puzzle module I2C address
#define PB_ADDR_MOD_DUMMY 0x69

/// \}
