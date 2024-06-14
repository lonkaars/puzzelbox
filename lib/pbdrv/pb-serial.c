#include <mpack.h>

#include "pb-serial.h"
#include "pb-types.h"

void pb_ser_w(mpack_writer_t * writer, const pb_msg_t * msg) {
	mpack_write_u8(writer, msg->type);
	mpack_write_u8(writer, msg->action);
	// TODO: if pb is compiled under the pb-mod target, place
	// PB_MOD_ADDR in this field
	mpack_write_u16(writer, msg->sender);

	if (msg->msg == NULL) return;
	switch (msg->type) {
		case PB_CMD_PROP: return pb_ser_w_cmd_prop(writer, msg);
		// case PB_CMD_STATE: return pb_ser_w_cmd_state(writer, msg);
		case PB_CMD_MAGIC: return pb_ser_w_cmd_magic(writer, msg);
		default: break;
	}
}
void pb_ser_r(mpack_reader_t * reader, pb_msg_t * msg) {
	msg->type = mpack_expect_u8(reader);
	msg->action = mpack_expect_u8(reader);
	msg->sender = mpack_expect_u16(reader);

	switch (msg->type) {
		case PB_CMD_PROP: return pb_ser_r_cmd_prop(reader, msg);
		// case PB_CMD_STATE: return pb_ser_r_cmd_state(reader, msg);
		case PB_CMD_MAGIC: return pb_ser_r_cmd_magic(reader, msg);
		default: break;
	}
}
void pb_ser_free(pb_msg_t * msg) {
	if (msg == NULL) return;

	switch (msg->type) {
		case PB_CMD_PROP: return pb_ser_free_cmd_prop(msg);
		// case PB_CMD_STATE: return pb_ser_free_cmd_state(msg);
		case PB_CMD_MAGIC: return pb_ser_free_cmd_magic(msg);
		default: break;
	}
}

void pb_ser_w_cmd_prop(mpack_writer_t * writer, const pb_msg_t * _msg) {
	pb_cmd_prop_t * msg = _msg->msg;

	mpack_write_u8(writer, msg->propid);
	mpack_write_bin(writer, (char *) msg->value, msg->_value_size);
}
void pb_ser_r_cmd_prop(mpack_reader_t * reader, pb_msg_t * _msg) {
	pb_cmd_prop_t * msg = _msg->msg = malloc(sizeof(pb_cmd_prop_t));

	msg->propid = mpack_expect_u8(reader);
	msg->_value_size = mpack_expect_bin(reader);
	msg->value = (uint8_t *) mpack_read_bytes_alloc(reader, msg->_value_size);
	mpack_done_bin(reader);
}
void pb_ser_free_cmd_prop(pb_msg_t * _msg) {
	if (_msg->msg != NULL) {
		pb_cmd_prop_t * msg = _msg->msg;
		if (msg->value != NULL) {
			MPACK_FREE(msg->value);
			msg->value = NULL;
		}
		free(_msg->msg);
		_msg->msg = NULL;
	}
}

void pb_ser_w_cmd_magic(mpack_writer_t * writer, const pb_msg_t * _msg) {
	pb_cmd_magic_t * msg = _msg->msg;

	mpack_write_bin(writer, (char *) msg->magic, msg->_magic_size);
}
void pb_ser_r_cmd_magic(mpack_reader_t * reader, pb_msg_t * _msg) {
	pb_cmd_magic_t * msg = _msg->msg = malloc(sizeof(pb_cmd_magic_t));

	msg->_magic_size = mpack_expect_bin(reader);
	msg->magic = mpack_read_bytes_alloc(reader, msg->_magic_size);
	mpack_done_bin(reader);
}
void pb_ser_free_cmd_magic(pb_msg_t * _msg) {
	if (_msg->msg != NULL) {
		pb_cmd_magic_t * msg = _msg->msg;
		if (msg->magic != NULL) {
			MPACK_FREE(msg->magic);
			msg->magic = NULL;
		}
		free(_msg->msg);
		_msg->msg = NULL;
	}
}

