#pragma once

#include "pb-types.h"

/**
 * \ingroup pb_drv
 * \defgroup pb_drv_rp2040 RP2040
 * \brief Raspberry Pi Pico and Pico W driver
 *
 * \note This file is no longer inside `lib/pbdrv/drv/rp2040` as it is tightly
 * coupled to both the pico-sdk and FreeRTOS functions. I have tried to get
 * FreeRTOS to play nicely with the CMake subproject layout, but the pico-sdk
 * and the RP2040 port of FreeRTOS both rely on CMake's import() functionality,
 * which makes using FreeRTOS in a libary context extremely messy.
 *
 * \warning The workaround implemented in this driver was already kind of
 * messy, and **a different microcontroller should be used for the main
 * controller instead**. Please see the handover document for more details.
 *
 * \{
 */

#ifdef __cplusplus
extern "C" {
#endif

//! Puzzle bus driver setup
void pb_setup();

/**
 * \note While the RP2040's datasheet claims it supports multi-master
 * configurations by implementing bus arbitration, it does not natively support
 * a mode where it is configured as a (multi-)master with a slave address, such
 * that it can be addressed by other multi-masters. This function includes a
 * hacky workaround that teporarily sets the RP2040 to I2C master mode to send
 * a message, and then restores it back to slave mode.
 *
 * \warning This approach results in some received frames being (partially)
 * dropped in the time period between the invocation of this function and the
 * bus becoming idle (and the message is sent).
 */
void pb_i2c_send(i2c_addr_t addr, const uint8_t * buf, size_t sz);

/**
 * \brief Scan the bus for I2C slaves, and send handshake messages to ACK-ing
 * slaves.
 *
 * \note As a result of the RP2040 hardware limitations, this function is also
 * implemented in this file, even though it does not belong to the puzzle bus
 * driver.
 *
 * \warning In order to not miss any handshake responses, the bus should remain
 * busy during the entire scan. The \c nostop parameter of the \c
 * i2c_write_timeout_us() function from the pico-sdk does not seem to keep the
 * bus busy.
 */
void bus_scan();

/// \}

#ifdef __cplusplus
}
#endif

