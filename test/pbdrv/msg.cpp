#include <gtest/gtest.h>

#include "pb-msg.h"
#include "pb-send.h"
#include "pb-mod.h"

TEST(pb_msg_rw, cmd_req_read) {
	pb_cmd_prop_t cmd = {
		.propid = 2,
	};
	pb_msg_t msg_write = {
		.type = PB_CMD_PROP,
		.action = PB_ACTION_REQ,
		.sender = 0xff,
		.cmd = &cmd,
	};
	pb_buf_t buf = pb_msg_write(&msg_write);

	ASSERT_NE(buf.data, nullptr);
	ASSERT_GE(buf.size, 0);

	pb_msg_t * msg_read = pb_msg_read(&buf);
	pb_buf_free(&buf);

	ASSERT_EQ(buf.data, nullptr);

	EXPECT_EQ(msg_write.type, msg_read->type);
	EXPECT_EQ(msg_write.sender, msg_read->sender);
	EXPECT_NE(nullptr, msg_read->cmd);
	pb_cmd_prop_t * prop = (pb_cmd_prop_t *) msg_read->cmd;
	EXPECT_EQ(prop->propid, cmd.propid);

	pb_msg_free(msg_read);
}

TEST(pb_msg_rw, cmd_req_magic) {
	pb_buf_t buf = pb_send_req_magic();

	ASSERT_NE(buf.data, nullptr);
	ASSERT_GE(buf.size, 0);

	pb_msg_t * msg_read = pb_msg_read(&buf);
	pb_buf_free(&buf);

	ASSERT_EQ(buf.data, nullptr);

	EXPECT_EQ(msg_read->type, PB_CMD_MAGIC);
	EXPECT_EQ(msg_read->sender, PB_MOD_ADDR);
	EXPECT_NE(msg_read->cmd, nullptr);
	pb_cmd_magic_t * magic = (pb_cmd_magic_t *) msg_read->cmd;
	EXPECT_EQ(magic->_magic_size, sizeof(pb_cmd_magic_req));
	EXPECT_EQ(0, memcmp(pb_cmd_magic_req, magic->magic, magic->_magic_size));

	pb_msg_free(msg_read);
}

