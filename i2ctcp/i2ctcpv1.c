#include <mpack.h>
#include <stdio.h>

// MIN() macro
#include <sys/param.h>

#include "i2ctcpv1.h"

int i2ctcp_read(i2ctcp_msg_t * target, const char * buf, size_t buf_sz) {
	// at start of message
	if (target->_rdata == 0) {
		// NOTE: The entire start of a message needs to be readable from the buffer
		// at this point. When target->addr can be read and target->length is past
		// the end of the current buffer block, this function will crash and burn.
		// This is a highly unlikely scenario, as i2ctcp_read is called for each
		// chunk of a TCP frame, and frames (should) include only one puzzle bus
		// message. The check here is kind of optional.
		if (buf_sz < 4) return -1;

		// mpack reader is used for the first buffer block, as it contains the data
		// size info
		mpack_reader_t reader;
		mpack_reader_init_data(&reader, buf, buf_sz);

		target->addr = mpack_expect_u16(&reader);
		target->length = target->_rdata = mpack_expect_bin(&reader);
		target->data = (char *) malloc(target->length);

		// read remaining data in (header) packet
		size_t to_read = mpack_reader_remaining(&reader, NULL);
		mpack_read_bytes(&reader, target->data, to_read);
		target->_rdata -= to_read;
	} else {
		// continue reading chunks of target->data until the amount of bytes
		// specified in target->length
		size_t to_read = MIN(buf_sz, target->_rdata);
		char * data = target->data + target->length - target->_rdata;
		memcpy(data, buf, to_read);
		target->_rdata -= to_read;
	}

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

