#pragma once

#include "../../pb-types.h"

typedef struct {
	i2c_addr_t mod_addr;
	pb_global_state_t mod_state;
} pb_mod_main_prop_mod_t;

enum {
	PB_MOD_MAIN_PROP_MODS = 0x00, //!< structured list of connected puzzle modules
};

