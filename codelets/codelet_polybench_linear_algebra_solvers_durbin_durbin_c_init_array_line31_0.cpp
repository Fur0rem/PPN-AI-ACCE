/**
 * @file codelets/codelet_polybench_durbin_c_init_array_line31_0.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 0;
int n = 2000;
double r[2000 + 0];

BENCHMARK_FN(durbin_c_init_array_line31_loop_c_0_c, {
	int __i_0__ = i;

	{
		for (__i_0__ = 0; __i_0__ <= n - 1; __i_0__ += 1) {
			r[__i_0__] = (n + 1 - __i_0__);
		}
	}

	i = __i_0__;
})

/**
 * @brief Run the benchmarks for durbin_c_init_array_line31_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(durbin_c_init_array_line31_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}