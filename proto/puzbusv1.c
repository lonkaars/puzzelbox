#include <mpack.h>
#include <stdio.h>

#include "puzbusv1.h"

int pb_read(struct pb_msg* target, char* buf, size_t buf_sz) {
	mpack_reader_t reader;
	printf("read %lu bytes...\n", buf_sz);

	mpack_reader_init_data(&reader, buf, buf_sz);

	uint16_t address = mpack_expect_u16(&reader);
	char data_buf[80];
	size_t data_size = mpack_expect_bin_buf(&reader, data_buf, sizeof(data_buf));

	printf("0x%02x\n", address);
	printf("\"%.*s\"\n", data_size, data_buf);

	return 0;
}

int pb_write(struct pb_msg* target, char** buf, size_t* buf_sz) {
	mpack_writer_t writer;
	mpack_writer_init_growable(&writer, buf, buf_sz);

	mpack_write_u16(&writer, target->addr);
	mpack_write_bin(&writer, target->data, target->length);

	// finish writing
	return mpack_writer_destroy(&writer) == mpack_ok;
}

