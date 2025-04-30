/**
 * @file codelets/codelet_polybench_atax_c_init_array_line34_0.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 0;
int n = 2100;
double x[2100 + 0];
double fn = 2100.000000;

BENCHMARK_FN(atax_c_init_array_line34_loop_c_0_c, {
	int __i_0__ = i;

	{
		for (__i_0__ = 0; __i_0__ <= n - 1; __i_0__ += 1) {
			x[__i_0__] = 1 + __i_0__ / fn;
		}
	}

	i = __i_0__;
})

/**
 * @brief Run the benchmarks for atax_c_init_array_line34_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(atax_c_init_array_line34_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}