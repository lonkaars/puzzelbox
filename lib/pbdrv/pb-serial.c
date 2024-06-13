#include <mpack.h>

#include "pb-serial.h"
#include "pb-types.h"

void pbdrv_serialize(mpack_writer_t * writer, const pb_msg_t * msg) {
	pbdrv_s_msg_header(writer, msg);
	if (msg->msg == NULL) return;

	switch (msg->type) {
		case PB_CMD_REQ_READ: return pbdrv_s_cmd_req_read(writer, msg->msg);
		case PB_CMD_RES_READ: return pbdrv_s_cmd_res_read(writer, msg->msg);
		default: break;
	}
}

void pbdrv_s_msg_header(mpack_writer_t * writer, const pb_msg_t * msg) {
	mpack_write_u8(writer, msg->type);
	// TODO: if pbdrv is compiled under the pbdrv-mod target, place
	// PBDRV_MOD_ADDR in this field
	mpack_write_u16(writer, msg->sender);
}

void pbdrv_s_cmd_req_read(mpack_writer_t * writer, const pb_cmd_req_read_t * msg) {
	mpack_write_u8(writer, msg->propid);
}

void pbdrv_s_cmd_res_read(mpack_writer_t * writer, const pb_cmd_res_read_t * msg) {
	mpack_write_u8(writer, msg->propid);
	mpack_write_bin(writer, (char *) msg->value, msg->_value_size);
}

