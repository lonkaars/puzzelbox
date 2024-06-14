#include <mpack.h>

#include "pb-msg.h"
#include "pb-serial.h"

pbdrv_buf_t pbdrv_msg_write(const pb_msg_t * msg) {
	pbdrv_buf_t buf = { 0 };
	mpack_writer_t writer;

	mpack_writer_init_growable(&writer, &buf.data, &buf.size);

	pbdrv_serialize(&writer, msg);

	mpack_writer_destroy(&writer);
	return buf;
}

pb_msg_t * pbdrv_msg_read(const pbdrv_buf_t * buf) {
	mpack_reader_t reader;
	mpack_reader_init_data(&reader, buf->data, buf->size);

	pb_msg_t * msg = malloc(sizeof(pb_msg_t));

	pbdrv_deserialize(&reader, msg);

	return msg;
}

void pbdrv_msg_free(pb_msg_t * msg) {
	pbdrv_free(msg);
}

