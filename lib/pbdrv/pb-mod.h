#pragma once

/**
 * \file puzzle bus driver implementation
 *
 * Most \c pb_* functions have a weak implementation, which may be overwritten
 * by a custom implementation. This allows you to use the default
 * implementation where possible, and only implement extensions required for
 * your puzzle module. Please see spec.adoc for more information about how to
 * use the puzzle bus driver library.
 */

#include "pb-types.h"

#ifdef __cplusplus
extern "C" {
#endif

//! puzzle module name (optional, default = "???")
extern const char * PB_MOD_NAME;
//! puzzle module bus address (required)
extern const i2c_addr_t PB_MOD_ADDR;

/**
 * \brief handle a received message from the I2C bus (puzzle bus)
 *
 * This function attempts to parse an I2C message as a puzzle bus message, and
 * calls the appropriate handler for the message if it is considered valid.
 *
 * \param buf pointer to message content
 * \param sz size of \p buf
 *
 * \note This function should not be directly called from an ISR. Please use
 * FreeRTOS's \c xTimerPendFunctionCallFromISR() or a similar scheduler-based
 * deferred function call mechanism instead.
 */
void pb_i2c_recv(const uint8_t * buf, size_t sz);
/**
 * \brief send a message in master-mode on the I2C bus (puzzle bus)
 *
 * This function sends an I2C message to the address specified by \p i2c_addr.
 *
 * \param i2c_addr address of slave controller
 * \param buf pointer to message content
 * \param sz size of \p buf
 */
void pb_i2c_send(i2c_addr_t i2c_addr, const uint8_t * buf, size_t sz);

/**
 * \brief global state read hook
 * \ingroup hook
 * \return current value of global state enum
 */
pb_global_state_t pb_hook_mod_state_read();
/**
 * \brief global state write hook
 * \ingroup hook
 * \param state new value of global state enum
 */
void pb_hook_mod_state_write(pb_global_state_t state);

/**
 * \brief platform-specific blocking delay function
 *
 * FIXME: this should be removed (see handover: RP2040 I2C limitations)
 */
void pb_mod_blocking_delay_ms(unsigned long ms);

#ifdef __cplusplus
}
#endif

