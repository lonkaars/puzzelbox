#include <gtest/gtest.h>

#include "pb-write.h"
#include "test.h"

// sorry for metaprogramming
#define test_write_fn(fn) \
	pbdrv_buf_t cmd_##fn; \
	TEST(pbdrv, write_cmd_##fn) { \
		cmd_##fn = pbdrv_write_cmd_##fn(expected_##fn); \
		ASSERT_NE(cmd_##fn.data, nullptr); \
		ASSERT_GE(cmd_##fn.size, 0); \
	}

test_write_fn(req_read);
test_write_fn(res_read);
test_write_fn(req_write);
test_write_fn(req_state);
test_write_fn(res_state);
test_write_fn(req_set_state);
test_write_fn(magic);

