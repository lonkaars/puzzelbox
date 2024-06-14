#pragma once

#include "pb-types.h"

/**
 * This is the RP2040 puzzle bus driver. This file is no longer inside
 * lib/pb//rp2040 as it is tightly coupled to both the pico-sdk and
 * freertos functions. I have tried to get FreeRTOS to play nicely with the
 * CMake subproject layout, but the pico-sdk and the rp2040 port of freertos
 * both rely on CMake's import() functionality, which makes using FreeRTOS in a
 * libary context extremely messy.
 *
 * The workaround implemented in this driver was already kind of messy, and a
 * different microcontroller should be used for the main controller instead.
 */

#ifdef __cplusplus
extern "C" {
#endif

//! puzzle bus driver setup
void pb_setup();

/**
 * While the RP2040's datasheet claims it supports multi-master configurations
 * by implementing bus arbitration, it does not natively support a mode where
 * it is configured as a (multi-)master with a slave address, such that it can
 * be addressed by other multi-masters. This function includes a hacky
 * workaround that teporarily sets the RP2040 to I2C master mode to send a
 * message, and then restores it back to slave mode.
 *
 * This approach results in some received frames being (partially) dropped in
 * the time period between the invocation of this function and the bus becoming
 * idle (and the message is sent).
 */
void pb_i2c_send(i2c_addr_t addr, const uint8_t * buf, size_t sz);

#ifdef __cplusplus
}
#endif

