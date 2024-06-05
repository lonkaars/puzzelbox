#pragma once

/**
 * \file puzzle bus driver implementation
 *
 * Most \c pbdrv_* functions have a weak implementation, which may be
 * overwritten by a custom implementation. This allows you to use the default
 * implementation where possible, and only implement extensions required for
 * your puzzle module. Please see spec.adoc for more information about how to
 * use the puzzle bus driver library.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "types.h"

extern const char * PBDRV_MOD_NAME;
extern const i2c_addr_t PBDRV_MOD_ADDR;

#ifdef __cplusplus
extern "C" {
#endif

void pbdrv_i2c_recv(const uint8_t * buf, size_t sz);
void pbdrv_i2c_send(i2c_addr_t i2c_addr, const uint8_t * buf, size_t sz);

#ifdef __cplusplus
}
#endif

