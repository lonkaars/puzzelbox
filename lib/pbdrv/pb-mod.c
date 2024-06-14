#include "pb-types.h"
#include "pb.h"

//! fallback module name
__weak const char * PB_MOD_NAME = "???";

//! [private] placeholder global state variable
static pb_global_state_t _global_state = PB_GS_NOINIT;

//! [private] main controller global state
static pb_global_state_t _main_state = PB_GS_NOINIT;

// __weak enum pb_state pb_hook_mod_state_read() {
// 	return _global_state;
// }

// __weak void pb_hook_mod_state_write(enum pb_state state) {
// 	_global_state = state;
// }

__weak void pb_i2c_recv(const uint8_t * buf, size_t sz) {
	return;
}
 
__weak void pb_hook_main_state_update(pb_global_state_t state) { }

