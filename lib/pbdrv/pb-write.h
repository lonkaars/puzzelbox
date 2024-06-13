#pragma once

#include <mpack.h>

#include "pb-types.h"

#ifdef __cplusplus
extern "C" {
#endif

pbdrv_buf_t pbdrv_write_msg(const pb_msg_t * msg);

#ifdef __cplusplus
}
#endif

