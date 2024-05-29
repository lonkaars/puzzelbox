#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void pbdrv_i2c_recv(uint16_t addr, const char * buf, size_t sz);
void pbdrv_i2c_send(uint16_t addr, const char * buf, size_t sz);

void pbdrv_hook_state(enum pb_global_state * state, bool rw);
bool pbdrv_hook_cmd(enum pb_cmd cmd, const char * buf, size_t sz);

void pbdrv_handle_read(const char * buf, size_t sz);

#ifdef __cplusplus
}
#endif

