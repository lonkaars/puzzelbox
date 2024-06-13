#pragma once

#include <mpack.h>

#include "pb-types.h"

#ifdef __cplusplus
extern "C" {
#endif


void pbdrv_serialize(mpack_writer_t * writer, const pb_msg_t * msg);

void pbdrv_s_msg_header(mpack_writer_t * writer, const pb_msg_t * msg);
void pbdrv_s_cmd_req_read(mpack_writer_t * writer, const pb_cmd_req_read_t * msg);
void pbdrv_s_cmd_res_read(mpack_writer_t * writer, const pb_cmd_res_read_t * msg);
void pbdrv_s_cmd_req_write(mpack_writer_t * writer, const pb_cmd_req_write_t * msg);
void pbdrv_s_cmd_req_state(mpack_writer_t * writer, const pb_cmd_req_state_t * msg);
void pbdrv_s_cmd_res_state(mpack_writer_t * writer, const pb_cmd_res_state_t * msg);
void pbdrv_s_cmd_req_set_state(mpack_writer_t * writer, const pb_cmd_req_set_state_t * msg);
void pbdrv_s_cmd_req_magic(mpack_writer_t * writer, const pb_cmd_req_magic_t * msg);
void pbdrv_s_cmd_res_magic(mpack_writer_t * writer, const pb_cmd_res_magic_t * msg);

// typedef void pbdrv_deserialize_t(mpack_reader_t * reader);
// pbdrv_deserialize_t pbdrv_d_msg_header;
// pbdrv_deserialize_t pbdrv_d_cmd_req_read;
// pbdrv_deserialize_t pbdrv_d_cmd_res_read;
// pbdrv_deserialize_t pbdrv_d_cmd_req_write;
// pbdrv_deserialize_t pbdrv_d_cmd_req_state;
// pbdrv_deserialize_t pbdrv_d_cmd_res_state;
// pbdrv_deserialize_t pbdrv_d_cmd_req_set_state;
// pbdrv_deserialize_t pbdrv_d_cmd_req_magic;
// pbdrv_deserialize_t pbdrv_d_cmd_res_magic;

#ifdef __cplusplus
}
#endif

