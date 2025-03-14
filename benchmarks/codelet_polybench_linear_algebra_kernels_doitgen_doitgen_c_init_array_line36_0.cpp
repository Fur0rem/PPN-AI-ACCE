/**
 * @file benchmarks/codelet_polybench_doitgen_c_init_array_line36_0.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 150;
int np = 160;
int j = 140;
double c4[160 + 0][160 + 0];

BENCHMARK_FN(doitgen_c_init_array_line36_loop_c_0_c, {
	int __i_0__ = i;
	int __j_1__ = j;

	{
		for (__i_0__ = 0; __i_0__ <= np - 1; __i_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= np - 1; __j_1__ += 1) {
				c4[__i_0__][__j_1__] = ((double)(__i_0__ * __j_1__ % np)) / np;
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for doitgen_c_init_array_line36_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(doitgen_c_init_array_line36_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}