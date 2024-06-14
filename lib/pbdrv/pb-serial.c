#include <mpack.h>

#include "pb-serial.h"
#include "pb-types.h"

void pb_ser_w(mpack_writer_t * writer, const pb_msg_t * cmd) {
	if (cmd == NULL) return;

	mpack_write_u8(writer, cmd->type);
	mpack_write_u8(writer, cmd->action);
	mpack_write_u16(writer, cmd->sender);
	if (cmd->cmd == NULL) return;

	switch (cmd->type) {
		case PB_CMD_PROP: return pb_ser_w_cmd_prop(writer, cmd);
		case PB_CMD_STATE: return pb_ser_w_cmd_state(writer, cmd);
		case PB_CMD_MAGIC: return pb_ser_w_cmd_magic(writer, cmd);
		default: break;
	}
}
void pb_ser_r(mpack_reader_t * reader, pb_msg_t * cmd) {
	cmd->type = mpack_expect_u8(reader);
	cmd->action = mpack_expect_u8(reader);
	cmd->sender = mpack_expect_u16(reader);

	switch (cmd->type) {
		case PB_CMD_PROP: return pb_ser_r_cmd_prop(reader, cmd);
		case PB_CMD_STATE: return pb_ser_r_cmd_state(reader, cmd);
		case PB_CMD_MAGIC: return pb_ser_r_cmd_magic(reader, cmd);
		default: break;
	}
}
void pb_ser_free(pb_msg_t * cmd) {
	if (cmd == NULL) return;

	switch (cmd->type) {
		case PB_CMD_PROP: return pb_ser_free_cmd_prop(cmd);
		case PB_CMD_STATE: return pb_ser_free_cmd_state(cmd);
		case PB_CMD_MAGIC: return pb_ser_free_cmd_magic(cmd);
		default: break;
	}
}

void pb_ser_w_cmd_prop(mpack_writer_t * writer, const pb_msg_t * _msg) {
	pb_cmd_prop_t * cmd = _msg->cmd;

	mpack_write_u8(writer, cmd->propid);
	mpack_write_bin(writer, (char *) cmd->value, cmd->_value_size);
}
void pb_ser_r_cmd_prop(mpack_reader_t * reader, pb_msg_t * _msg) {
	pb_cmd_prop_t * cmd = _msg->cmd = malloc(sizeof(pb_cmd_prop_t));

	cmd->propid = mpack_expect_u8(reader);
	cmd->_value_size = mpack_expect_bin(reader);
	cmd->value = (uint8_t *) mpack_read_bytes_alloc(reader, cmd->_value_size);
	mpack_done_bin(reader);
}
void pb_ser_free_cmd_prop(pb_msg_t * _msg) {
	if (_msg->cmd != NULL) {
		pb_cmd_prop_t * cmd = _msg->cmd;
		if (cmd->value != NULL) {
			MPACK_FREE(cmd->value);
			cmd->value = NULL;
		}
		free(_msg->cmd);
		_msg->cmd = NULL;
	}
}

void pb_ser_w_cmd_state(mpack_writer_t * writer, const pb_msg_t * _msg) {
	pb_cmd_state_t * cmd = _msg->cmd;

	mpack_write_u8(writer, cmd->state);
}
void pb_ser_r_cmd_state(mpack_reader_t * reader, pb_msg_t * _msg) {
	pb_cmd_state_t * cmd = _msg->cmd = malloc(sizeof(pb_cmd_state_t));

	cmd->state = mpack_expect_u8(reader);
}
void pb_ser_free_cmd_state(pb_msg_t * _msg) {
	if (_msg->cmd != NULL) {
		free(_msg->cmd);
		_msg->cmd = NULL;
	}
}

void pb_ser_w_cmd_magic(mpack_writer_t * writer, const pb_msg_t * _msg) {
	pb_cmd_magic_t * cmd = _msg->cmd;

	mpack_write_bin(writer, (char *) cmd->magic, cmd->_magic_size);
}
void pb_ser_r_cmd_magic(mpack_reader_t * reader, pb_msg_t * _msg) {
	pb_cmd_magic_t * cmd = _msg->cmd = malloc(sizeof(pb_cmd_magic_t));

	cmd->_magic_size = mpack_expect_bin(reader);
	cmd->magic = mpack_read_bytes_alloc(reader, cmd->_magic_size);
	mpack_done_bin(reader);
}
void pb_ser_free_cmd_magic(pb_msg_t * _msg) {
	if (_msg->cmd != NULL) {
		pb_cmd_magic_t * cmd = _msg->cmd;
		if (cmd->magic != NULL) {
			MPACK_FREE(cmd->magic);
			cmd->magic = NULL;
		}
		free(_msg->cmd);
		_msg->cmd = NULL;
	}
}

