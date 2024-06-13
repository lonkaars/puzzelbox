#include <mpack.h>

#include "pb-write.h"
#include "pb-serial.h"

pbdrv_buf_t pbdrv_write_msg(const pb_msg_t * msg) {
	pbdrv_buf_t buf = { 0 };
	mpack_writer_t writer;

	mpack_writer_init_growable(&writer, &buf.data, &buf.size);

	pbdrv_serialize(&writer, msg);

	mpack_writer_destroy(&writer);
	return buf;
}

// 
// pbdrv_buf_t pbdrv_write_cmd_req_read(pb_cmd_req_read_t data) {
// 	data.header.type = PB_CMD_REQ_READ;
// 	pbdrv_writer_t writer;
// 	pbdrv_write_init(&writer);
// 
// 	pbdrv_write_msg_header(&writer, data.header);
// 	mpack_write_u8(&writer.writer, data.propid);
// 
// 	return pbdrv_write_finish(&writer);
// }
// 
// pbdrv_buf_t pbdrv_write_cmd_res_read(pb_cmd_res_read_t data) {
// 	data.header.type = PB_CMD_RES_READ;
// 	pbdrv_writer_t writer;
// 	pbdrv_write_init(&writer);
// 
// 	pbdrv_write_msg_header(&writer, data.header);
// 	mpack_write_u8(&writer.writer, data.propid);
// 	mpack_write_bin(&writer.writer, (char *) data.value, data._value_size);
// 
// 	return pbdrv_write_finish(&writer);
// }
// 
// pbdrv_buf_t pbdrv_write_cmd_req_write(pb_cmd_req_write_t data) {
// 	data.header.type = PB_CMD_REQ_WRITE;
// 	pbdrv_writer_t writer;
// 	pbdrv_write_init(&writer);
// 
// 	pbdrv_write_msg_header(&writer, data.header);
// 	mpack_write_u8(&writer.writer, data.propid);
// 	mpack_write_bin(&writer.writer, (char *) data.value, data._value_size);
// 
// 	return pbdrv_write_finish(&writer);
// }
// 
// pbdrv_buf_t pbdrv_write_cmd_req_state(pb_cmd_req_state_t data) {
// 	data.header.type = PB_CMD_REQ_STATE;
// 	pbdrv_writer_t writer;
// 	pbdrv_write_init(&writer);
// 
// 	pbdrv_write_msg_header(&writer, data.header);
// 	mpack_write_u8(&writer.writer, data.state);
// 
// 	return pbdrv_write_finish(&writer);
// }
// 
// pbdrv_buf_t pbdrv_write_cmd_res_state(pb_cmd_res_state_t data) {
// 	data.header.type = PB_CMD_RES_STATE;
// 	pbdrv_writer_t writer;
// 	pbdrv_write_init(&writer);
// 
// 	pbdrv_write_msg_header(&writer, data.header);
// 	mpack_write_u8(&writer.writer, data.state);
// 
// 	return pbdrv_write_finish(&writer);
// }
// 
// pbdrv_buf_t pbdrv_write_cmd_req_set_state(pb_cmd_req_set_state_t data) {
// 	data.header.type = PB_CMD_REQ_SET_STATE;
// 	pbdrv_writer_t writer;
// 	pbdrv_write_init(&writer);
// 
// 	pbdrv_write_msg_header(&writer, data.header);
// 	mpack_write_u8(&writer.writer, data.state);
// 
// 	return pbdrv_write_finish(&writer);
// }
// 
// pbdrv_buf_t pbdrv_write_cmd_magic(pb_cmd_magic_t data) {
// 	data.header.type = PB_CMD_MAGIC;
// 	pbdrv_writer_t writer;
// 	pbdrv_write_init(&writer);
// 
// 	pbdrv_write_msg_header(&writer, data.header);
// 	mpack_write_bin(&writer.writer, data.magic, data._magic_size);
// 
// 	return pbdrv_write_finish(&writer);
// }

