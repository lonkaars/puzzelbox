#include <gtest/gtest.h>

#include "pb-msg.h"

TEST(pbdrv_msg_rw, cmd_req_read) {
	pb_cmd_req_read_t content = {
		.propid = 2,
	};
	pb_msg_t msg_write = {
		.type = PB_CMD_REQ_READ,
		.sender = 0xff,
		.msg = &content,
	};
	pbdrv_buf_t buf = pbdrv_msg_write(&msg_write);

	ASSERT_NE(buf.data, nullptr);
	ASSERT_GE(buf.size, 0);

	pb_msg_t * msg_read = pbdrv_msg_read(&buf);
	pbdrv_buf_free(&buf);

	ASSERT_EQ(buf.data, nullptr);

	EXPECT_EQ(msg_write.type, msg_read->type);
	EXPECT_EQ(msg_write.sender, msg_read->sender);
	EXPECT_EQ(((pb_cmd_req_read_t *) msg_write.msg)->propid, ((pb_cmd_req_read_t *) msg_read->msg)->propid);

	pbdrv_msg_free(msg_read);
}

