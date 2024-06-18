#include <mpack.h>

#include "pb-write.h"

typedef struct {
	mpack_writer_t writer;
	pbdrv_buf_t buf;
} pbdrv_writer_t;

static pbdrv_writer_t pbdrv_write_init() {
	pbdrv_writer_t writer;
	mpack_writer_init_growable(&writer.writer, &writer.buf.data, &writer.buf.size);
	return writer;
}

static pbdrv_buf_t pbdrv_write_finish(pbdrv_writer_t * writer) {
	if (mpack_writer_destroy(&writer->writer) != mpack_ok) {
		writer->buf.data = NULL;
		writer->buf.size = 0;
	}
	return writer->buf;
}

static void pbdrv_write_msg_header(pbdrv_writer_t * writer, pb_msg_header_t header) {
	mpack_write_u8(&writer->writer, header.type);
	mpack_write_u16(&writer->writer, header.sender);
}

pbdrv_buf_t pbdrv_write_cmd_req_set_state(pb_cmd_req_set_state_t data) {
	pbdrv_writer_t writer = pbdrv_write_init();
	pbdrv_write_msg_header(&writer, data.header);
	mpack_write_u8(&writer.writer, data.state);
	return pbdrv_write_finish(&writer);
}

