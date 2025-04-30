/**
 * @file codelets/codelet_polybench_ludcmp_c_init_array_line35_0.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 0;
int n = 2000;
double x[2000 + 0];
double y[2000 + 0];
double b[2000 + 0];
double fn = 2000.000000;

BENCHMARK_FN(ludcmp_c_init_array_line35_loop_c_0_c, {
	int __i_0__ = i;

	{
		for (__i_0__ = 0; __i_0__ <= n - 1; __i_0__ += 1) {
			x[__i_0__] = 0;
			y[__i_0__] = 0;
			b[__i_0__] = (__i_0__ + 1) / fn / 2.0 + 4;
		}
	}

	i = __i_0__;
})

/**
 * @brief Run the benchmarks for ludcmp_c_init_array_line35_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(ludcmp_c_init_array_line35_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}