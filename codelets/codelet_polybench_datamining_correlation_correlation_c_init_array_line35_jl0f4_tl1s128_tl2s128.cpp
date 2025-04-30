/**
 * @file codelets/codelet_polybench_correlation_c_init_array_line35_jl0f4_tl1s128_tl2s128.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 0;
int j = 0;
double data[1400 + 0][1200 + 0];

BENCHMARK_FN(correlation_c_init_array_line35_loop_c_8_c, {
	int __i_0__ = i;
	int __j_1__ = j;

	{
		int _lt_var___i_0__;
		for (_lt_var___i_0__ = 0; _lt_var___i_0__ <= 1399; _lt_var___i_0__ += 512) {
			int _lt_var___j_1__;
			for (_lt_var___j_1__ = 0; _lt_var___j_1__ <= 1199; _lt_var___j_1__ += 128) {
				for (__i_0__ = _lt_var___i_0__; __i_0__ <= (((1399 < (_lt_var___i_0__ + 128 - 1)) ? 1399 : (_lt_var___i_0__ + 128 - 1)));
					 __i_0__ += 4) {
					for (__j_1__ = _lt_var___j_1__;
						 __j_1__ <= (((1199 < (_lt_var___j_1__ + 128 - 1)) ? 1199 : (_lt_var___j_1__ + 128 - 1)));
						 __j_1__ += 1) {
						data[__i_0__][__j_1__] = ((double)(__i_0__ * __j_1__)) / ((double)1200) + ((double)__i_0__);
						data[__i_0__ + 1][__j_1__] = ((double)((__i_0__ + 1) * __j_1__)) / ((double)1200) + ((double)(__i_0__ + 1));
						data[__i_0__ + 2][__j_1__] = ((double)((__i_0__ + 2) * __j_1__)) / ((double)1200) + ((double)(__i_0__ + 2));
						data[__i_0__ + 3][__j_1__] = ((double)((__i_0__ + 3) * __j_1__)) / ((double)1200) + ((double)(__i_0__ + 3));
					}
				}
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for correlation_c_init_array_line35_loop_c_8_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(correlation_c_init_array_line35_loop_c_8_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}