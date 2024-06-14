#include "pb-send.h"
#include "pb-mod.h"
#include "pb-msg.h"

pb_buf_t pb_send_req_read(uint8_t propid) {
	pb_cmd_prop_t cmd = {
		.propid = propid,
		.value = NULL,
		._value_size = 0,
	};
	pb_msg_t msg = {
		.type = PB_CMD_PROP,
		.action = PB_ACTION_REQ,
		.sender = PB_MOD_ADDR,
		.cmd = &cmd,
	};
	return pb_msg_write(&msg);
}

pb_buf_t pb_send_res_read(uint8_t propid, uint8_t * value, size_t size) {
	pb_cmd_prop_t cmd = {
		.propid = propid,
		.value = value,
		._value_size = size,
	};
	pb_msg_t msg = {
		.type = PB_CMD_PROP,
		.action = PB_ACTION_RES,
		.sender = PB_MOD_ADDR,
		.cmd = &cmd,
	};
	return pb_msg_write(&msg);
}

pb_buf_t pb_send_req_write(uint8_t propid, uint8_t * value, size_t size) {
	pb_cmd_prop_t cmd = {
		.propid = propid,
		.value = value,
		._value_size = size,
	};
	pb_msg_t msg = {
		.type = PB_CMD_PROP,
		.action = PB_ACTION_REQ,
		.sender = PB_MOD_ADDR,
		.cmd = &cmd,
	};
	return pb_msg_write(&msg);
}

pb_buf_t pb_send_req_state() {
	pb_cmd_state_t cmd = {
		.state = pb_hook_mod_state_read(),
	};
	pb_msg_t msg = {
		.type = PB_CMD_STATE,
		.action = PB_ACTION_REQ,
		.sender = PB_MOD_ADDR,
		.cmd = &cmd,
	};
	return pb_msg_write(&msg);
}

pb_buf_t pb_send_res_state(pb_global_state_t state) {
	pb_cmd_state_t cmd = {
		.state = state,
	};
	pb_msg_t msg = {
		.type = PB_CMD_STATE,
		.action = PB_ACTION_RES,
		.sender = PB_MOD_ADDR,
		.cmd = &cmd,
	};
	return pb_msg_write(&msg);
}

pb_buf_t pb_send_req_set_state(pb_global_state_t state) {
	pb_cmd_state_t cmd = {
		.state = state,
	};
	pb_msg_t msg = {
		.type = PB_CMD_STATE,
		.action = PB_ACTION_SET,
		.sender = PB_MOD_ADDR,
		.cmd = &cmd,
	};
	return pb_msg_write(&msg);
}

pb_buf_t pb_send_req_magic() {
	pb_cmd_magic_t cmd = {
		.magic = (char *) &pb_cmd_magic_req[0],
		._magic_size = sizeof(pb_cmd_magic_req),
	};
	pb_msg_t msg = {
		.type = PB_CMD_MAGIC,
		.action = PB_ACTION_REQ,
		.sender = PB_MOD_ADDR,
		.cmd = &cmd,
	};
	return pb_msg_write(&msg);
}

pb_buf_t pb_send_res_magic() {
	pb_cmd_magic_t cmd = {
		.magic = (char *) &pb_cmd_magic_res[0],
		._magic_size = sizeof(pb_cmd_magic_res),
	};
	pb_msg_t msg = {
		.type = PB_CMD_MAGIC,
		.action = PB_ACTION_RES,
		.sender = PB_MOD_ADDR,
		.cmd = &cmd,
	};
	return pb_msg_write(&msg);
}

