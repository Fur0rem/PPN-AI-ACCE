/**
 * @file codelets/codelet_polybench_covariance_c_kernel_covariance_line81_0.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 1400;
int n = 1400;
int j = 1200;
int m = 1200;
double data[1400 + 0][1200 + 0];
double mean[1200 + 0];

BENCHMARK_FN(covariance_c_kernel_covariance_line81_loop_c_0_c, {
	int __i_0__ = i;
	int __j_1__ = j;

	{
		for (__i_0__ = 0; __i_0__ <= n - 1; __i_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= m - 1; __j_1__ += 1) {
				data[__i_0__][__j_1__] -= mean[__j_1__];
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for covariance_c_kernel_covariance_line81_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(3, oss).RUN(covariance_c_kernel_covariance_line81_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}