#pragma once

#include "../types.h"

typedef struct __packed {
	const uint8_t addr;
	const enum pb_state state;
} pb_mod_main_mod_t;

enum __packed {
	PB_MOD_MAIN_ADDR_MODS = 0x01, //!< connected puzzle modules
};

