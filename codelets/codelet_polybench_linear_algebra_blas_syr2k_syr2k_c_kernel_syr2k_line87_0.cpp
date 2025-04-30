/**
 * @file codelets/codelet_polybench_syr2k_c_kernel_syr2k_line87_0.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 0;
int n = 1200;
int j = 0;
double c[1200 + 0][1200 + 0];
double beta = 1.200000;

BENCHMARK_FN(syr2k_c_kernel_syr2k_line87_loop_c_0_c, {
	int __i_0__ = i;
	int __j_1__ = j;

	{
		for (__i_0__ = 0; __i_0__ <= n - 1; __i_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= n - 1; __j_1__ += 1) {
				c[__i_0__][__j_1__] *= beta;
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for syr2k_c_kernel_syr2k_line87_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(3, oss).RUN(syr2k_c_kernel_syr2k_line87_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}