#include "pb-buf.h"
#include "pb-mem.h"

void pb_buf_free(pb_buf_t * buf) {
	if (buf->data == NULL) return;
	pb_free(buf->data);
	buf->data = NULL;
}
