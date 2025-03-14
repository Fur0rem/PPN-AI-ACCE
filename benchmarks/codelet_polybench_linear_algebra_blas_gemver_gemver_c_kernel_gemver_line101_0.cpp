/**
 * @file benchmarks/codelet_polybench_gemver_c_kernel_gemver_line101_0.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 0;
int n = 2000;
int j = 0;
double a[2000 + 0][2000 + 0];
double u1[2000 + 0];
double v1[2000 + 0];
double u2[2000 + 0];
double v2[2000 + 0];

BENCHMARK_FN(gemver_c_kernel_gemver_line101_loop_c_0_c, {
	int __i_0__ = i;
	int __j_1__ = j;

	{
		for (__i_0__ = 0; __i_0__ <= n - 1; __i_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= n - 1; __j_1__ += 1) {
				a[__i_0__][__j_1__] = a[__i_0__][__j_1__] + u1[__i_0__] * v1[__j_1__] + u2[__i_0__] * v2[__j_1__];
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for gemver_c_kernel_gemver_line101_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(3, oss).RUN(gemver_c_kernel_gemver_line101_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}