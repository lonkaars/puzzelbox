#include <mpack.h>

#include "pb-read.h"

// pb_msg_header_t * pbdrv_read_msg_header(mpack_reader_t * reader, pb_msg_header_t * header) {
// 	header->type = mpack_expect_u8(reader);
// 	header->sender = mpack_expect_u16(reader);
// 
// 	return header;
// }
// 
// pb_cmd_req_read_t * pbdrv_read_cmd_req_read(mpack_reader_t * reader, pb_cmd_req_read_t * msg) {
// 	msg->propid = mpack_expect_u8(reader);
// 
// 	return msg;
// }
// 
// void * pbdrv_read_unknown(const char * data, size_t size) {
// 	mpack_reader_t reader;
// 	mpack_reader_init_data(&reader, data, size);
// 
// 	void * msg = NULL;
// 
// 	pb_msg_header_t header;
// 	pbdrv_read_msg_header(&reader, &header);
// 	switch (header.type) {
// 		case PB_CMD_REQ_READ: {
// 			// pb_cmd_req_read_t msg;
// 			msg = malloc(sizeof(pb_cmd_req_read_t));
// 			pbdrv_read_cmd_req_read(&reader, msg);
// 			break;
// 		}
// 		default: return NULL;
// 	}
// 
// 	if (mpack_reader_error(&reader) != mpack_ok) return NULL;
// 	return NULL;
// }

