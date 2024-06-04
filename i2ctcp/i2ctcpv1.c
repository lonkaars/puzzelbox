#include <mpack.h>
#include <sys/param.h> // MIN() macro

#include "i2ctcpv1.h"

int i2ctcp_read(i2ctcp_msg_t * target, const char * buf, size_t buf_sz) {
	mpack_reader_t reader;
	mpack_reader_init_data(&reader, buf, buf_sz);

	// at start of message
	if (target->_rdata == 0) {
		target->addr = mpack_expect_u16(&reader);
		target->length = target->_rdata = mpack_expect_bin(&reader);
		if (mpack_reader_error(&reader) != mpack_ok) return -1;
		target->data = (char *) malloc(target->length);

		// seek forward in buf to where binary data begins (to avoid having to read
		// from private member reader.data in the memcpy below)
		buf += buf_sz - mpack_reader_remaining(&reader, NULL);
	}

	// continue reading chunks of target->data until the amount of bytes
	// specified in target->length
	size_t to_read = MIN(mpack_reader_remaining(&reader, NULL), target->_rdata);
	char * data = target->data + target->length - target->_rdata;
	memcpy(data, buf, to_read);
	target->_rdata -= to_read;
	// NOTE: memcpy is used here because mpack_read_bytes requires that a tag was
	// opened, which is not the case for the chunks following the initial mpack
	// header

	// if rdata = 0, the message was completely read
	return target->_rdata;
}

void i2ctcp_read_reset(i2ctcp_msg_t * target) {
	target->_rdata = 0;
}

bool i2ctcp_write(const i2ctcp_msg_t * target, char ** buf, size_t * buf_sz) {
	mpack_writer_t writer;
	mpack_writer_init_growable(&writer, buf, buf_sz);

	mpack_write_u16(&writer, target->addr);
	mpack_write_bin(&writer, target->data, target->length);

	// finish writing
	return mpack_writer_destroy(&writer) == mpack_ok;
}

