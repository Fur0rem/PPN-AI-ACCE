/**
 * @file codelets/codelet_polybench_jacobi_1d_c_init_array_line32_0.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 0;
int n = 2000;
double a[2000 + 0];
double b[2000 + 0];

BENCHMARK_FN(jacobi_1d_c_init_array_line32_loop_c_0_c, {
	int __i_0__ = i;

	{
		for (__i_0__ = 0; __i_0__ <= n - 1; __i_0__ += 1) {
			a[__i_0__] = (((double)__i_0__) + 2) / n;
			b[__i_0__] = (((double)__i_0__) + 3) / n;
		}
	}

	i = __i_0__;
})

/**
 * @brief Run the benchmarks for jacobi_1d_c_init_array_line32_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(jacobi_1d_c_init_array_line32_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}