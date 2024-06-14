#pragma once

#include "pb-types.h"
#include "pb-buf.h"

#ifdef __cplusplus
extern "C" {
#endif

pb_buf_t pb_msg_write(const pb_msg_t * msg);
pb_msg_t * pb_msg_read(const pb_buf_t * buf);
void pb_msg_free(pb_msg_t * msg);

// pb_buf_t pb_msg_write_cmd_req_read(const pb_cmd_req_read_t);
// pb_buf_t pb_msg_write_cmd_res_read(const pb_cmd_res_read_t);
// pb_buf_t pb_msg_write_cmd_req_write
// pb_buf_t pb_msg_write_cmd_req_state
// pb_buf_t pb_msg_write_cmd_res_state
// pb_buf_t pb_msg_write_cmd_req_set_state
// pb_buf_t pb_msg_write_cmd_req_magic
// pb_buf_t pb_msg_write_cmd_res_magic

pb_buf_t pb_msg_write_req_magic();
pb_buf_t pb_msg_write_res_magic();

#ifdef __cplusplus
}
#endif

