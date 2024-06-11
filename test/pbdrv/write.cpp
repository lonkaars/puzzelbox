#include <gtest/gtest.h>

#include "pb-write.h"

TEST(pbdrv, write_cmd_req_read) {
	pbdrv_buf_t buf = pbdrv_write_cmd_req_read({
		.propid = 0,
	});

	ASSERT_NE(buf.data, nullptr);
	ASSERT_GE(buf.size, 0);
}

TEST(pbdrv, write_cmd_res_read) {
	pbdrv_buf_t buf = pbdrv_write_cmd_res_read({
		.propid = 0,
		.value = (uint8_t[]) { 0x00, },
		._value_size = 1,
	});

	ASSERT_NE(buf.data, nullptr);
	ASSERT_GE(buf.size, 0);
}

TEST(pbdrv, write_cmd_req_write) {
	pbdrv_buf_t buf = pbdrv_write_cmd_req_write({
		.propid = 0,
		.value = (uint8_t[]) { 0x00, },
		._value_size = 1,
	});

	ASSERT_NE(buf.data, nullptr);
	ASSERT_GE(buf.size, 0);
}

TEST(pbdrv, write_cmd_req_state) {
	pbdrv_buf_t buf = pbdrv_write_cmd_req_state({
		.state = PB_GS_PLAYING,
	});

	ASSERT_NE(buf.data, nullptr);
	ASSERT_GE(buf.size, 0);
}

TEST(pbdrv, write_cmd_res_state) {
	pbdrv_buf_t buf = pbdrv_write_cmd_res_state({
		.state = PB_GS_IDLE,
	});

	ASSERT_NE(buf.data, nullptr);
	ASSERT_GE(buf.size, 0);
}

TEST(pbdrv, write_cmd_req_set_state) {
	pbdrv_buf_t buf = pbdrv_write_cmd_req_set_state({
		.state = PB_GS_PLAYING,
	});

	ASSERT_NE(buf.data, nullptr);
	ASSERT_GE(buf.size, 0);
}

TEST(pbdrv, write_cmd_magic) {
	pbdrv_buf_t buf = pbdrv_write_cmd_magic({
		.magic = pb_cmd_magic_msg,
		._magic_size = sizeof(pb_cmd_magic_msg),
	});

	ASSERT_NE(buf.data, nullptr);
	ASSERT_GE(buf.size, 0);
}

