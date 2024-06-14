#pragma once

#include "../pb-types.h"

typedef struct {
	const i2c_addr_t mod_addr;
	const pb_global_state_t mod_state;
} pb_mod_main_mod_t;

enum {
	PB_MOD_MAIN_ADDR_MODS = 0x01, //!< connected puzzle modules
};

