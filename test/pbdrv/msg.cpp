#include <gtest/gtest.h>

#include "pb-msg.h"

TEST(pb_msg_rw, cmd_req_read) {
	pb_cmd_prop_t content = {
		.propid = 2,
	};
	pb_msg_t msg_write = {
		.type = PB_CMD_PROP,
		.action = PB_ACTION_REQ,
		.sender = 0xff,
		.msg = &content,
	};
	pb_buf_t buf = pb_msg_write(&msg_write);

	ASSERT_NE(buf.data, nullptr);
	ASSERT_GE(buf.size, 0);

	pb_msg_t * msg_read = pb_msg_read(&buf);
	pb_buf_free(&buf);

	ASSERT_EQ(buf.data, nullptr);

	EXPECT_EQ(msg_write.type, msg_read->type);
	EXPECT_EQ(msg_write.sender, msg_read->sender);
	EXPECT_NE(nullptr, msg_read->msg);
	pb_cmd_prop_t * prop = (pb_cmd_prop_t *) msg_read->msg;
	EXPECT_EQ(prop->propid, content.propid);

	pb_msg_free(msg_read);
}

TEST(pb_msg_rw, cmd_req_magic) {
	pb_buf_t buf = pb_msg_write_req_magic();

	ASSERT_NE(buf.data, nullptr);
	ASSERT_GE(buf.size, 0);

	pb_msg_t * msg_read = pb_msg_read(&buf);
	pb_buf_free(&buf);

	ASSERT_EQ(buf.data, nullptr);

	EXPECT_EQ(msg_read->type, PB_CMD_MAGIC);
	EXPECT_EQ(msg_read->sender, 0);
	EXPECT_NE(msg_read->msg, nullptr);
	pb_cmd_magic_t * magic = (pb_cmd_magic_t *) msg_read->msg;
	EXPECT_EQ(magic->_magic_size, sizeof(pb_cmd_magic_req));
	EXPECT_EQ(0, memcmp(pb_cmd_magic_req, magic->magic, magic->_magic_size));

	pb_msg_free(msg_read);
}

