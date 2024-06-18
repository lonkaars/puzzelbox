#include "mpack.h"

size_t mpack_read_remaining_bytes(mpack_reader_t * reader, char * p, size_t count) {
	size_t limit = mpack_reader_remaining(reader, NULL);
	if (0 < count && count < limit)
		limit = count;
	MPACK_MEMCPY(p, reader->data, limit);
	return limit;
}

