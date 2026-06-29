#include "math.hpp"

namespace mymath {

auto add(int a, int b) -> int {
	return a + b;
}

auto subtract(int a, int b) -> int {
	return a - b;
}

auto multiply(int a, int b) -> int {
	return a * b;
}

auto divide(double a, double b) -> double {
	if (b == 0) {
		return 0; // Error case
	}
	return a / b;
}

} // namespace mymath
