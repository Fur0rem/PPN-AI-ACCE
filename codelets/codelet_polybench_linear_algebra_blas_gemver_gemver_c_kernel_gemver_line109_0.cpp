/**
 * @file codelets/codelet_polybench_gemver_c_kernel_gemver_line109_0.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 2000;
int n = 2000;
double x[2000 + 0];
double z[2000 + 0];

BENCHMARK_FN(gemver_c_kernel_gemver_line109_loop_c_0_c, {
	int __i_0__ = i;

	{
		for (__i_0__ = 0; __i_0__ <= n - 1; __i_0__ += 1) {
			x[__i_0__] = x[__i_0__] + z[__i_0__];
		}
	}

	i = __i_0__;
})

/**
 * @brief Run the benchmarks for gemver_c_kernel_gemver_line109_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(3, oss).RUN(gemver_c_kernel_gemver_line109_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}