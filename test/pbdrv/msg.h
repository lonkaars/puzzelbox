#pragma once

#include "pb-types.h"

static const pb_cmd_req_read_t expected_req_read = {
	.propid = 0,
};

static const pb_cmd_res_read_t expected_res_read = {
	.propid = 0,
	.value = (uint8_t[]) { 0x00, },
	._value_size = 1,
};

static const pb_cmd_req_write_t expected_req_write = {
	.propid = 0,
	.value = (uint8_t[]) { 0x00, },
	._value_size = 1,
};

static const pb_cmd_req_state_t expected_req_state = {
	.state = PB_GS_PLAYING,
};

static const pb_cmd_res_state_t expected_res_state = {
	.state = PB_GS_IDLE,
};

static const pb_cmd_req_set_state_t expected_req_set_state = {
	.state = PB_GS_PLAYING,
};
