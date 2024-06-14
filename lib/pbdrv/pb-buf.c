#include "pb-buf.h"

#include <stdlib.h>

void pb_buf_free(pb_buf_t * buf) {
	if (buf->data == NULL) return;
	free(buf->data);
	buf->data = NULL;
}

