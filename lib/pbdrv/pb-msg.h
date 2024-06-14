#pragma once

#include "pb-types.h"
#include "pb-buf.h"

#ifdef __cplusplus
extern "C" {
#endif

pbdrv_buf_t pbdrv_msg_write(const pb_msg_t * msg);
pb_msg_t * pbdrv_msg_read(const pbdrv_buf_t * buf);
void pbdrv_msg_free(pb_msg_t * msg);

#ifdef __cplusplus
}
#endif

