/**
 * @file codelets/codelet_polybench_bicg_c_init_array_line33_0.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 0;
int m = 1900;
double p[1900 + 0];

BENCHMARK_FN(bicg_c_init_array_line33_loop_c_0_c, {
	int __i_0__ = i;

	{
		for (__i_0__ = 0; __i_0__ <= m - 1; __i_0__ += 1) {
			p[__i_0__] = ((double)(__i_0__ % m)) / m;
		}
	}

	i = __i_0__;
})

/**
 * @brief Run the benchmarks for bicg_c_init_array_line33_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(bicg_c_init_array_line33_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}