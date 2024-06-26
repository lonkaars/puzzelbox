#include <gtest/gtest.h>

#include "pb-mem.h"

TEST(pb_mem, all) {
	void * chunk = pb_malloc(64);
	ASSERT_NE(chunk, nullptr);

	pb_free(chunk);
}
