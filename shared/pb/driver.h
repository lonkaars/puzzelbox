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

#ifndef PBDRV_MOD_NAME
#define PBDRV_MOD_NAME "???"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void pbdrv_i2c_recv(uint16_t i2c_addr, const char * buf, size_t sz);
void pbdrv_i2c_send(uint16_t i2c_addr, const char * buf, size_t sz);

pb_state_t pbdrv_hook_mod_state_read();
void pbdrv_hook_mod_state_write(pb_state_t state);
void pbdrv_hook_main_state_update(pb_state_t state);

/**
 * \name hooks
 *
 * Implementing this function allows you to use the weak implementation of \c
 * pbdrv_i2c_recv() while being able to implement custom command handlers.
 *
 * \return true if the cmd was recognized, or false to forward the command to
 * the default handlers
 *
 * \{
 */

/** \brief cmd receive hook */
bool pbdrv_hook_cmd(uint16_t i2c_addr, pb_cmd_t cmd, const char * buf, size_t sz);
/** \brief read cmd hook */
bool pbdrv_hook_read(uint16_t i2c_addr, uint8_t addr);
/** \brief write cmd hook */
bool pbdrv_hook_write(uint16_t i2c_addr, uint8_t addr, const char * buf, size_t sz);
//! \}

void pbdrv_handle_read(uint16_t i2c_addr, const char * buf, size_t sz);
void pbdrv_handle_write(uint16_t i2c_addr, const char * buf, size_t sz);
void pbdrv_handle_magic(uint16_t i2c_addr, const char * buf, size_t sz);
void pbdrv_handle_sex(uint16_t i2c_addr, const char * buf, size_t sz);

#ifdef __cplusplus
}
#endif

