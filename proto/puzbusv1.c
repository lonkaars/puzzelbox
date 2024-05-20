#include <mpack.h>
#include <stdio.h>

// MIN() macro
#include <sys/param.h>
// TODO: check if this works on pico as well

#include "puzbusv1.h"

/**
 * \brief Remaining bytes to be read to target->data
 *
 * This is the only variable that needs to persist between buffer blocks. It is
 * declared in the global scope to allow resetting using the \c pb_read_reset()
 * function.
 *
 * \note \p rdata may be reset by calling \c pb_read_reset()
 */
static size_t rdata = 0;

int pb_read(struct pb_msg* target, char* buf, size_t buf_sz) {
	// a new reader is used per buffer block passed to this function
	mpack_reader_t reader;
	mpack_reader_init_data(&reader, buf, buf_sz);

	// at start of message
	if (rdata == 0) {
		// NOTE: The entire start of a message needs to be readable from the buffer
		// at this point. When target->addr can be read and target->length is past
		// the end of the current buffer block, this function will crash and burn.
		// This is a highly unlikely scenario, as pb_read is called for each chunk
		// of a TCP frame, and frames (should) include only one puzzle bus message.
		// The check here is kind of optional.
		if (buf_sz < 4) return -1;

		target->addr = mpack_expect_u16(&reader);
		target->length = rdata = mpack_expect_bin(&reader);
		target->data = (char*) malloc(target->length);
	}

	// continue reading chunks of target->data until the amount of bytes
	// specified in target->length
	size_t to_read = MIN(mpack_reader_remaining(&reader, NULL), rdata);
	char* data = target->data + target->length - rdata; // 'ol pointer arithmetic
	mpack_read_bytes(&reader, data, to_read);
	rdata -= to_read;

	// if rdata = 0, the message was completely read
	return rdata;
}

void pb_read_reset() {
	rdata = 0;
}

bool pb_write(struct pb_msg* target, char** buf, size_t* buf_sz) {
	mpack_writer_t writer;
	mpack_writer_init_growable(&writer, buf, buf_sz);

	mpack_write_u16(&writer, target->addr);
	mpack_write_bin(&writer, target->data, target->length);

	// finish writing
	return mpack_writer_destroy(&writer) == mpack_ok;
}

