#include "pb-buf.h"

#include <stdlib.h>

void pbdrv_buf_free(pbdrv_buf_t * buf) {
	if (buf->data == NULL) return;
	free(buf->data);
	buf->data = NULL;
}

