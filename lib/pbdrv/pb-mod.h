#pragma once

/**
 * \file puzzle bus driver implementation
 *
 * Most \c pb_* functions have a weak implementation, which may be
 * overwritten by a custom implementation. This allows you to use the default
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

void pb_i2c_recv(const uint8_t * buf, size_t sz);
void pb_i2c_send(i2c_addr_t i2c_addr, const uint8_t * buf, size_t sz);

pb_global_state_t pb_hook_mod_state_read();
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

