#pragma once

#include <stdint.h>
#include <stddef.h>
#include <mpack.h>

#include "pb-types.h"

#ifdef __cplusplus
extern "C" {
#endif

// pb_msg_header_t * pbdrv_read_msg_header(mpack_reader_t * reader, pb_msg_header_t * header);
// pb_cmd_req_read_t * pbdrv_read_cmd_req_read(mpack_reader_t * reader, pb_cmd_req_read_t * msg);
// 
// pb_cmd_res_read_t * pbdrv_read_cmd_res_read(const char * data, size_t size);
// pb_cmd_req_write_t * pbdrv_read_cmd_req_write(const char * data, size_t size);
// pb_cmd_req_state_t * pbdrv_read_cmd_req_state(const char * data, size_t size);
// pb_cmd_res_state_t * pbdrv_read_cmd_res_state(const char * data, size_t size);
// pb_cmd_req_set_state_t * pbdrv_read_cmd_req_set_state(const char * data, size_t size);
// pb_cmd_magic_t * pbdrv_read_cmd_magic(const char * data, size_t size);

pb_msg_t * pbdrv_read_unknown(const pbdrv_buf_t * buf);

#ifdef __cplusplus
}
#endif

