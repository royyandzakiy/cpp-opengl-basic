#include "../src/math.hpp"
#include <gtest/gtest.h>

using namespace mymath; // functions are declared in namespace mymath

TEST(MathTest, Add) {
	EXPECT_EQ(add(2, 3), 5);
	EXPECT_EQ(add(-1, 1), 0);
	EXPECT_EQ(add(0, 0), 0);
}

TEST(MathTest, Subtract) {
	EXPECT_EQ(subtract(10, 4), 6);
	EXPECT_EQ(subtract(0, 5), -5);
}

TEST(MathTest, Multiply) {
	EXPECT_EQ(multiply(4, 5), 20);
	EXPECT_EQ(multiply(0, 10), 0);
	EXPECT_EQ(multiply(-3, 4), -12);
}

TEST(MathTest, Divide) {
	EXPECT_DOUBLE_EQ(divide(10, 2), 5.0);
	EXPECT_DOUBLE_EQ(divide(5, 0), 0.0);
	EXPECT_DOUBLE_EQ(divide(7, 2), 3.5);
}