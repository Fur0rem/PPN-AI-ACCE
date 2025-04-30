/**
 * @file codelets/codelet_polybench_mvt_c_kernel_mvt_line88_0.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 0;
int n = 2000;
int j = 0;
double x1[2000 + 0];
double A[2000 + 0][2000 + 0];
double y_1[2000 + 0];

BENCHMARK_FN(mvt_c_kernel_mvt_line88_loop_c_0_c, {
	int __i_0__ = i;
	int __j_1__ = j;

	{
		for (__i_0__ = 0; __i_0__ <= n - 1; __i_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= n - 1; __j_1__ += 1) {
				x1[__i_0__] = x1[__i_0__] + A[__i_0__][__j_1__] * y_1[__j_1__];
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for mvt_c_kernel_mvt_line88_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(3, oss).RUN(mvt_c_kernel_mvt_line88_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}