#pragma once

#include "pb-types.h"
#include "pb-buf.h"

#ifdef __cplusplus
extern "C" {
#endif

void pb_send_reply(pb_msg_t * msg, pb_buf_t * reply);

pb_buf_t pb_send_read_req(uint8_t propid);
pb_buf_t pb_send_read_res(uint8_t propid, uint8_t * value, size_t size);
pb_buf_t pb_send_write_req(uint8_t propid, uint8_t * value, size_t size);
pb_buf_t pb_send_state_req();
pb_buf_t pb_send_state_res();
pb_buf_t pb_send_state_set(pb_global_state_t state);
pb_buf_t pb_send_magic_req();
pb_buf_t pb_send_magic_res();

#ifdef __cplusplus
}
#endif

