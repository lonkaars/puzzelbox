#include <gtest/gtest.h>

#include "pb-mod.h"
#include "pb-msg.h"
#include "pb-send.h"

/**
 * Only one of each command type is tested due to the (de)serializers being the
 * same (i.e. you're not testing more by changing the command action).
 */

TEST(pb_send_rw, cmd_state_req) {
	pb_hook_mod_state_write(PB_GS_NOINIT);

	pb_buf_t buf = pb_send_state_req();
	ASSERT_NE(buf.data, nullptr);
	ASSERT_GE(buf.size, 0);

	pb_msg_t * msg = pb_msg_read(&buf);
	EXPECT_EQ(msg->type, PB_CMD_STATE);
	EXPECT_EQ(msg->action, PB_ACTION_REQ);
	EXPECT_EQ(msg->sender, PB_MOD_ADDR);
	EXPECT_NE(nullptr, msg->cmd);

	pb_cmd_state_t * cmd = (pb_cmd_state_t *) msg->cmd;
	EXPECT_EQ(cmd->state, pb_hook_mod_state_read());

	pb_buf_free(&buf);
	ASSERT_EQ(buf.data, nullptr);

	pb_msg_free(msg);
}

TEST(pb_send_rw, cmd_read_req) {
	pb_buf_t buf = pb_send_read_req(2);
	ASSERT_NE(buf.data, nullptr);
	ASSERT_GE(buf.size, 0);

	pb_msg_t * msg = pb_msg_read(&buf);

	EXPECT_EQ(msg->type, PB_CMD_PROP);
	EXPECT_EQ(msg->action, PB_ACTION_REQ);
	EXPECT_EQ(msg->sender, PB_MOD_ADDR);
	EXPECT_NE(nullptr, msg->cmd);

	pb_cmd_prop_t * cmd = (pb_cmd_prop_t *) msg->cmd;
	EXPECT_EQ(cmd->propid, 2);

	pb_buf_free(&buf);
	ASSERT_EQ(buf.data, nullptr);

	pb_msg_free(msg);
}

TEST(pb_send_rw, cmd_magic_req) {
	pb_buf_t buf = pb_send_magic_req();
	ASSERT_NE(buf.data, nullptr);
	ASSERT_GE(buf.size, 0);

	pb_msg_t * msg = pb_msg_read(&buf);

	EXPECT_EQ(msg->type, PB_CMD_MAGIC);
	EXPECT_EQ(msg->action, PB_ACTION_REQ);
	EXPECT_EQ(msg->sender, PB_MOD_ADDR);
	EXPECT_NE(msg->cmd, nullptr);

	pb_cmd_magic_t * cmd = (pb_cmd_magic_t *) msg->cmd;
	EXPECT_EQ(cmd->_magic_size, sizeof(pb_cmd_magic_req));
	EXPECT_EQ(0, memcmp(pb_cmd_magic_req, cmd->magic, cmd->_magic_size));

	pb_buf_free(&buf);
	ASSERT_EQ(buf.data, nullptr);

	pb_msg_free(msg);
}
