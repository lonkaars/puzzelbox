#pragma once

#include "pb-types.h"
#include "pb-buf.h"

#ifdef __cplusplus
extern "C" {
#endif

pb_buf_t pb_msg_write(const pb_msg_t * msg);
pb_msg_t * pb_msg_read(const pb_buf_t * buf);
void pb_msg_free(pb_msg_t * msg);

#ifdef __cplusplus
}
#endif

