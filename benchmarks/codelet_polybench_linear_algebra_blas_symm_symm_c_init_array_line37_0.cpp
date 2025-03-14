/**
 * @file benchmarks/codelet_polybench_symm_c_init_array_line37_0.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 0;
int m = 1000;
int j = 0;
int n = 1200;
double c[1000 + 0][1200 + 0];
double b[1000 + 0][1200 + 0];

BENCHMARK_FN(symm_c_init_array_line37_loop_c_0_c, {
	int __i_0__ = i;
	int __j_1__ = j;

	{
		NO_VECTORISE_NO_UNROLL
		for (__i_0__ = 0; __i_0__ <= m - 1; __i_0__ += 1) {
			NO_VECTORISE_NO_UNROLL
			for (__j_1__ = 0; __j_1__ <= n - 1; __j_1__ += 1) {
				c[__i_0__][__j_1__] = ((double)((__i_0__ + __j_1__) % 100)) / m;
				b[__i_0__][__j_1__] = ((double)((n + __i_0__ - __j_1__) % 100)) / m;
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for symm_c_init_array_line37_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(symm_c_init_array_line37_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}