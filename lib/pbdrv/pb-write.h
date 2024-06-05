#pragma once

#include "pb-types.h"

#ifdef __cplusplus
extern "C" {
#endif

//! binary buffer struct
typedef struct {
	char * data; //! pointer to data
	size_t size; //! size of data
} pbdrv_buf_t;

pbdrv_buf_t pbdrv_write_cmd_req_read(pb_cmd_req_read_t data);
pbdrv_buf_t pbdrv_write_cmd_res_read(pb_cmd_res_read_t data);
pbdrv_buf_t pbdrv_write_cmd_req_write(pb_cmd_req_write_t data);
pbdrv_buf_t pbdrv_write_cmd_req_state(pb_cmd_req_state_t data);
pbdrv_buf_t pbdrv_write_cmd_res_state(pb_cmd_res_state_t data);
pbdrv_buf_t pbdrv_write_cmd_req_set_state(pb_cmd_req_set_state_t data);
pbdrv_buf_t pbdrv_write_cmd_magic(pb_cmd_magic_t data);

#ifdef __cplusplus
}
#endif

