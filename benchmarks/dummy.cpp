#define ANKERL_NANOBENCH_IMPLEMENT
#include "../nanobench/src/include/nanobench.h"

int func_to_benchmark(int left, int right) {
	return left + right;
}

int main() {
	ankerl::nanobench::Bench().run("Dummy benchmark", [&] {
		ankerl::nanobench::doNotOptimizeAway(func_to_benchmark(0, 0));
	});

	return 0;
}
