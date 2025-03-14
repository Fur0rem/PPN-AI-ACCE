/**
 * @file benchmarks/codelet_livermore_default_lloops_c_kernels_line971_0.cpp
 * @brief Benchmark extracted from the Livermore loops codelet
 */

#include "bench_common.hpp"

long i = 0;
long inner_loops = 101;
double px[101][25];
double xtra[40];

BENCHMARK_FN(lloops_c_kernels_line971_loop_c_0_c, {
	int __i_0__ = i;

	{
		for (__i_0__ = 0; __i_0__ < inner_loops; __i_0__++) {
			px[__i_0__][0] = xtra[22] * px[__i_0__][12] + xtra[21] * px[__i_0__][11] + xtra[20] * px[__i_0__][10] +
							 xtra[19] * px[__i_0__][9] + xtra[18] * px[__i_0__][8] + xtra[17] * px[__i_0__][7] + xtra[16] * px[__i_0__][6] +
							 xtra[12] * (px[__i_0__][4] + px[__i_0__][5]) + px[__i_0__][2];
		}
	}

	i = __i_0__;
})

/**
 * @brief Run the benchmarks for lloops_c_kernels_line971_main_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(lloops_c_kernels_line971_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
