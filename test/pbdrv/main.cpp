#include <gtest/gtest.h>

#include "pb-write.h"

TEST(pbdrv, write) {

	pbdrv_buf_t buf = pbdrv_write_cmd_req_set_state({
		.header = { .sender = 0xf0, },
		.state = PB_GS_PLAYING,
	});
	for (size_t i = 0; i < buf.size; i++) {
		printf("%02x ", buf.data[i] & 0xff);
	}
	printf("\n");

	ASSERT_TRUE(true);
}

