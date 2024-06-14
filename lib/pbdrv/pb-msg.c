#include <mpack.h>

#include "pb-msg.h"
#include "pb-serial.h"

pb_buf_t pb_msg_write(const pb_msg_t * msg) {
	pb_buf_t buf = { 0 };
	mpack_writer_t writer;

	mpack_writer_init_growable(&writer, &buf.data, &buf.size);

	pb_ser_w(&writer, msg);

	mpack_writer_destroy(&writer);
	return buf;
}

pb_msg_t * pb_msg_read(const pb_buf_t * buf) {
	mpack_reader_t reader;
	mpack_reader_init_data(&reader, buf->data, buf->size);

	pb_msg_t * msg = malloc(sizeof(pb_msg_t));

	pb_ser_r(&reader, msg);

	mpack_reader_destroy(&reader);
	return msg;
}

void pb_msg_free(pb_msg_t * msg) {
	// free message fields recursively
	pb_ser_free(msg);

	// free message container that was created in \p pb_msg_read
	free(msg);
}

pb_buf_t pb_msg_write_req_magic() {
	pb_cmd_magic_t content = {
		.magic = (char *) &pb_cmd_magic_req[0],
		._magic_size = sizeof(pb_cmd_magic_req),
	};
	pb_msg_t msg_write = {
		.type = PB_CMD_MAGIC,
		.action = PB_ACTION_REQ,
		.sender = 0,
		.msg = &content,
	};
	return pb_msg_write(&msg_write);
}

