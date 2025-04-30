/**
 * @file codelets/codelet_polybench_2mm_c_init_array_line38_0.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 0;
int ni = 800;
int j = 0;
int nk = 1100;
double a[800 + 0][1100 + 0];

BENCHMARK_FN(2mm_c_init_array_line38_loop_c_0_c, {
	int __i_0__ = i;
	int __j_1__ = j;

	{
		for (__i_0__ = 0; __i_0__ <= ni - 1; __i_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= nk - 1; __j_1__ += 1) {
				a[__i_0__][__j_1__] = ((double)(__i_0__ * __j_1__ % ni)) / ni;
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for 2mm_c_init_array_line38_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(2mm_c_init_array_line38_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}