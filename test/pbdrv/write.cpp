#include <gtest/gtest.h>

#include "pb-write.h"

TEST(pbdrv, write_cmd_req_set_state) {
	pbdrv_buf_t buf = pbdrv_write_cmd_req_set_state({
		.header = { .sender = 0xf0, },
		.state = PB_GS_PLAYING,
	});

	ASSERT_NE(buf.data, nullptr);
	ASSERT_GE(buf.size, 0);
}

