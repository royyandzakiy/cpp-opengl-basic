#include <gtest/gtest.h>

TEST(AlwaysTrue, True) {
	EXPECT_EQ(true, true);
}

auto main(int argc, char **argv) -> int {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}