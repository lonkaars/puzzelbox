#include <mpack.h>

#include "pb-serial.h"
#include "pb-types.h"

void pbdrv_serialize(mpack_writer_t * writer, const pb_msg_t * msg) {
	pbdrv_sr_msg_header(writer, msg);
	if (msg->msg == NULL) return;
	switch (msg->type) {
		case PB_CMD_REQ_READ: return pbdrv_sr_cmd_req_read(writer, msg);
		case PB_CMD_RES_READ: return pbdrv_sr_cmd_res_read(writer, msg);
		default: break;
	}
}
void pbdrv_deserialize(mpack_reader_t * reader, pb_msg_t * msg) {
	pbdrv_dsr_msg_header(reader, msg);
	switch (msg->type) {
		case PB_CMD_REQ_READ: return pbdrv_dsr_cmd_req_read(reader, msg);
		case PB_CMD_RES_READ: return pbdrv_dsr_cmd_res_read(reader, msg);
		default: break;
	}
}
void pbdrv_free(pb_msg_t * msg) {
	if (msg == NULL) return;

	switch (msg->type) {
		case PB_CMD_REQ_READ: return pbdrv_free_cmd_req_read(msg);
		case PB_CMD_RES_READ: return pbdrv_free_cmd_res_read(msg);
		default: break;
	}
	pbdrv_free_msg_header(msg);
}

void pbdrv_sr_msg_header(mpack_writer_t * writer, const pb_msg_t * msg) {
	mpack_write_u8(writer, msg->type);
	// TODO: if pbdrv is compiled under the pbdrv-mod target, place
	// PBDRV_MOD_ADDR in this field
	mpack_write_u16(writer, msg->sender);
}
void pbdrv_dsr_msg_header(mpack_reader_t * reader, pb_msg_t * msg) {
	msg->type = mpack_expect_u8(reader);
	msg->sender = mpack_expect_u16(reader);
}
void pbdrv_free_msg_header(pb_msg_t * msg) { }

void pbdrv_sr_cmd_req_read(mpack_writer_t * writer, const pb_msg_t * _msg) {
	pb_cmd_req_read_t * msg = _msg->msg;
	mpack_write_u8(writer, msg->propid);
}
void pbdrv_dsr_cmd_req_read(mpack_reader_t * reader, pb_msg_t * _msg) {
	pb_cmd_req_read_t * msg = _msg->msg = malloc(sizeof(pb_cmd_req_read_t));
	msg->propid = mpack_expect_u8(reader);
}
void pbdrv_free_cmd_req_read(pb_msg_t * _msg) {
	if (_msg->msg != NULL) {
		free(_msg->msg);
		_msg->msg = NULL;
	}
}

void pbdrv_sr_cmd_res_read(mpack_writer_t * writer, const pb_msg_t * _msg) {
	pb_cmd_res_read_t * msg = _msg->msg;

	mpack_write_u8(writer, msg->propid);
	mpack_write_bin(writer, (char *) msg->value, msg->_value_size);
}
void pbdrv_dsr_cmd_res_read(mpack_reader_t * reader, pb_msg_t * _msg) {
	pb_cmd_res_read_t * msg = _msg->msg = malloc(sizeof(pb_cmd_res_read_t));

	msg->propid = mpack_expect_u8(reader);
	msg->_value_size = mpack_expect_bin(reader);
	msg->value = (uint8_t *) mpack_read_bytes_alloc(reader, msg->_value_size);
	mpack_done_bin(reader);
}
void pbdrv_free_cmd_res_read(pb_msg_t * _msg) {
	if (_msg->msg != NULL) {
		pb_cmd_res_read_t * msg = _msg->msg;
		if (msg->value != NULL) {
			MPACK_FREE(msg->value);
			msg->value = NULL;
		}
		free(_msg->msg);
		_msg->msg = NULL;
	}
}

