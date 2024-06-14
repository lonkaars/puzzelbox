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

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "pb-buf.h"
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

void pb_reply(pb_msg_t * msg, pb_buf_t * reply);

pb_global_state_t pb_hook_mod_state_read();
void pb_hook_mod_state_write(pb_global_state_t state);

#ifdef __cplusplus
}
#endif

