#include "pb-route.h"
#include "pb-mod.h"
#include "pb-send.h"
#include "pb-types.h"
#include "pb-mem.h"

__weak bool pb_hook_route_msg(pb_msg_t * msg) { return false; }
__weak void pb_route_msg(pb_msg_t * msg) {
	if (pb_hook_route_msg(msg)) return;

	switch (msg->type) {
		case PB_CMD_PROP: return pb_route_cmd_prop(msg);
		case PB_CMD_STATE: return pb_route_cmd_state(msg);
		case PB_CMD_MAGIC: return pb_route_cmd_magic(msg);
		default: return;
	}
}

__weak bool pb_hook_route_cmd_prop(pb_msg_t * msg) { return false; }
__weak void pb_route_cmd_prop(pb_msg_t * msg) {
	if (pb_hook_route_cmd_prop(msg)) return;

	switch (msg->action) {
		case PB_ACTION_REQ: return pb_route_cmd_prop_req(msg);
		case PB_ACTION_RES: return pb_route_cmd_prop_res(msg);
		case PB_ACTION_SET: return pb_route_cmd_prop_set(msg);
		default: return;
	}
}

__weak bool pb_hook_route_cmd_state(pb_msg_t * msg) { return false; }
__weak void pb_route_cmd_state(pb_msg_t * msg) {
	if (pb_hook_route_cmd_state(msg)) return;

	switch (msg->action) {
		case PB_ACTION_REQ: return pb_route_cmd_state_req(msg);
		case PB_ACTION_RES: return pb_route_cmd_state_res(msg);
		case PB_ACTION_SET: return pb_route_cmd_state_set(msg);
		default: return;
	}
}

__weak bool pb_hook_route_cmd_magic(pb_msg_t * msg) { return false; }
__weak void pb_route_cmd_magic(pb_msg_t * msg) {
	if (pb_hook_route_cmd_magic(msg)) return;

	switch (msg->action) {
		case PB_ACTION_REQ: return pb_route_cmd_magic_req(msg);
		case PB_ACTION_RES: return pb_route_cmd_magic_res(msg);
		default: return;
	}
}

// all properties are user-defined
__weak void pb_route_cmd_prop_req(pb_msg_t * msg) {}
__weak void pb_route_cmd_prop_res(pb_msg_t * msg) {}
__weak void pb_route_cmd_prop_set(pb_msg_t * msg) {}

//! last known global state of last STATE REQ sender (i.e. main controller)
static pb_global_state_t _main_state = PB_GS_NOINIT;
__weak void pb_hook_ev_main_state_update(pb_global_state_t state) {}
__weak void pb_hook_ev_module_init() {
	pb_hook_mod_state_write(PB_GS_IDLE);
}
__weak void pb_route_cmd_state_req(pb_msg_t * msg) {
	pb_global_state_t own_state = pb_hook_mod_state_read();

	// notify of new global state variable
	pb_cmd_state_t * cmd = msg->cmd;
	if (cmd->state != _main_state) {
		// first STATE REQ = module init OK
		if (_main_state == PB_GS_NOINIT) pb_hook_ev_module_init();
		pb_hook_ev_main_state_update(cmd->state);
	}
	_main_state = cmd->state;

	pb_buf_t buf = pb_send_state_res();
	pb_send_reply(msg, &buf);
	pb_buf_free(&buf);
}

__weak void pb_route_cmd_state_res(pb_msg_t * msg) {}

__weak void pb_route_cmd_state_set(pb_msg_t * msg) {
	pb_cmd_state_t * cmd = msg->cmd;
	pb_hook_mod_state_write(cmd->state);
}

__weak void pb_route_cmd_magic_req(pb_msg_t * msg) {
	pb_cmd_magic_t * cmd = msg->cmd;
	// return early if magic has wrong size
	if (cmd->_magic_size != sizeof(pb_cmd_magic_req)) return;
	// // return early if magic doesn't match
	if (pb_memcmp(cmd->magic, pb_cmd_magic_req, sizeof(pb_cmd_magic_req)) != 0) return;

	// FIXME: this should be removed (see handover: RP2040 I2C limitations)
	pb_mod_blocking_delay_ms(2000);

	pb_buf_t buf = pb_send_magic_res();
	pb_send_reply(msg, &buf);
	pb_buf_free(&buf);
}

__weak void pb_route_cmd_magic_res(pb_msg_t * msg) { }

