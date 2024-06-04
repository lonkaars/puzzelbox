#include <gtest/gtest.h>

class ExampleTest : public testing::Test {
protected:
};

TEST_F(ExampleTest, Test) {
	EXPECT_EQ(5, 5);
}

