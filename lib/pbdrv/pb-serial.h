#pragma once

#include <mpack.h>

#include "pb-types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define __pb_cmd(name) \
	pb_ser_r_t pb_ser_r_##name; \
	pb_ser_w_t pb_ser_w_##name; \
	pb_ser_free_t pb_ser_free_##name;

typedef void pb_ser_w_t(mpack_writer_t * writer, const pb_msg_t * msg);
pb_ser_w_t pb_ser_w;

typedef void pb_ser_r_t(mpack_reader_t * reader, pb_msg_t * msg);
pb_ser_r_t pb_ser_r;

typedef void pb_ser_free_t(pb_msg_t * msg);
pb_ser_free_t pb_ser_free;

__pb_cmd(msg_header)
__pb_cmd(cmd_req_read)
__pb_cmd(cmd_res_read)
__pb_cmd(cmd_req_write)
__pb_cmd(cmd_req_state)
__pb_cmd(cmd_res_state)
__pb_cmd(cmd_req_set_state)
__pb_cmd(cmd_req_magic)
__pb_cmd(cmd_res_magic)

#ifdef __cplusplus
}
#endif

