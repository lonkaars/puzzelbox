#include <algorithm>
#include <gtest/gtest.h>

#include "i2ctcpv1.h"

using std::min;

const uint8_t data[] = {
	0xff, 0x00, 0xde, 0xad, 0xbe, 0xef,
};
const size_t data_len = sizeof(data);
const size_t chunk_size = 6;

char * send_data = nullptr;
size_t send_size = 0;

TEST(i2ctcp, send) {
	i2ctcp_msg_t send_msg = {
		.addr = 0x1122,
		.data = (char *) data,
		.length = data_len,
	};

	ASSERT_TRUE(i2ctcp_write(&send_msg, &send_data, &send_size));
	ASSERT_NE(send_data, nullptr);
	ASSERT_GE(send_size, 0);
}

TEST(i2ctcp, recv) {
	i2ctcp_msg_t recv_msg;
	i2ctcp_read_reset(&recv_msg);
	for (size_t i = 0; i < send_size; i += chunk_size) {
		size_t expected_size = min(send_size, i + chunk_size) - i;

		int parsed = i2ctcp_read(&recv_msg, send_data + i, expected_size);
		EXPECT_GE(parsed, 0);

		if (i + expected_size == send_size) EXPECT_EQ(parsed, 0);
		else EXPECT_GT(parsed, 0);
	}

	ASSERT_NE(recv_msg.data, nullptr);
	ASSERT_EQ(recv_msg.length, data_len);
	ASSERT_EQ(0, memcmp(recv_msg.data, data, data_len));
}
