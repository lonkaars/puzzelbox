#include <gtest/gtest.h>

#include "pb-write.h"
#include "pb-read.h"

#include "cmd.h"

// sorry for metaprogramming
#define test_cmd(fn) \
	TEST(pbdrv_cmd, fn) { \
		pbdrv_buf_t cmd = pbdrv_write_cmd_##fn(expected_##fn); \
		ASSERT_NE(cmd.data, nullptr); \
		ASSERT_GE(cmd.size, 0); \
	}

// test_cmd(req_read);
// test_cmd(res_read);
// test_cmd(req_write);
// test_cmd(req_state);
// test_cmd(res_state);
// test_cmd(req_set_state);
// test_cmd(magic);

TEST(pbdrv_cmd_rw, magic) {
	pb_cmd_req_read_t content = {
		.propid = 0,
	};
	pb_msg_t msg = {
		.type = PB_CMD_REQ_READ,
		.sender = 0xff,
		.msg = &content,
	};
	pbdrv_buf_t buf = pbdrv_write_msg(&msg);

	ASSERT_NE(buf.data, nullptr);
	ASSERT_GE(buf.size, 0);

	// void * _cmd = NULL;
	// pb_cmd_id type = pbdrv_read_msg();
	// ASSERT_EQ(type, PB_CMD_MAGIC);
}

// pb_cmd_req_read_t * msg = (pb_cmd_req_read_t *) pbdrv_read_unknown(cmd_req_read.data, cmd_req_read.size);
// ASSERT_NE(msg, nullptr);
// ASSERT_EQ(msg->header.type, PB_CMD_REQ_READ);

// TEST(pbdrv, read_cmd_res_read) { }
// TEST(pbdrv, read_cmd_req_state) { }
// TEST(pbdrv, read_cmd_res_state) { }
// TEST(pbdrv, read_cmd_req_set_state) { }
// TEST(pbdrv, read_cmd_magic) { }

