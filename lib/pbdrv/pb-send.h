#pragma once

#include "pb-types.h"
#include "pb-buf.h"

#ifdef __cplusplus
extern "C" {
#endif

pb_buf_t pb_send_req_read(uint8_t propid);
pb_buf_t pb_send_res_read(uint8_t propid, uint8_t * value, size_t size);
pb_buf_t pb_send_req_write(uint8_t propid, uint8_t * value, size_t size);
pb_buf_t pb_send_req_state();
pb_buf_t pb_send_res_state(pb_global_state_t state);
pb_buf_t pb_send_req_set_state(pb_global_state_t state);
pb_buf_t pb_send_req_magic();
pb_buf_t pb_send_res_magic();

#ifdef __cplusplus
}
#endif

