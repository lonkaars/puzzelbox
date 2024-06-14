#pragma once

#include <mpack.h>

#include "pb-types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define __pb_cmd(name) \
	pbdrv_serialize_t pbdrv_sr_##name; \
	pbdrv_deserialize_t pbdrv_dsr_##name; \
	pbdrv_free_t pbdrv_free_##name;

typedef void pbdrv_serialize_t(mpack_writer_t * writer, const pb_msg_t * msg);
pbdrv_serialize_t pbdrv_serialize;

typedef void pbdrv_deserialize_t(mpack_reader_t * reader, pb_msg_t * msg);
pbdrv_deserialize_t pbdrv_deserialize;

typedef void pbdrv_free_t(pb_msg_t * msg);
pbdrv_free_t pbdrv_free;

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

