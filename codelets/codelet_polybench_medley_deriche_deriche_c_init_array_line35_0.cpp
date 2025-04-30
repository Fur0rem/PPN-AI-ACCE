/**
 * @file codelets/codelet_polybench_deriche_c_init_array_line35_0.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 0;
int w = 4096;
int j = 0;
int h = 2160;
float img_in[4096 + 0][2160 + 0];

BENCHMARK_FN(deriche_c_init_array_line35_loop_c_0_c, {
	int __i_0__ = i;
	int __j_1__ = j;

	{
		for (__i_0__ = 0; __i_0__ <= w - 1; __i_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= h - 1; __j_1__ += 1) {
				img_in[__i_0__][__j_1__] = ((float)((313 * __i_0__ + 991 * __j_1__) % 65536)) / 65535.0f;
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for deriche_c_init_array_line35_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(deriche_c_init_array_line35_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}