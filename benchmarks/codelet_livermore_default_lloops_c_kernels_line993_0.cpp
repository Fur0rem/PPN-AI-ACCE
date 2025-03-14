/**
 * @file benchmarks/codelet_livermore_default_lloops_c_kernels_line993_0.cpp
 * @brief Benchmark extracted from the Livermore loops codelet
 */

#include "bench_common.hpp"

long i = 0;
long inner_loops = 101;
double ar = 0;
double cx[101][25];
double br = 0;
double px[101][25];
double cr = 0;

BENCHMARK_FN(lloops_c_kernels_line993_loop_c_0_c, {
	int __i_0__ = i;
	{
		for (__i_0__ = 0; __i_0__ < inner_loops; __i_0__++) {
			ar = cx[__i_0__][4];
			br = ar - px[__i_0__][4];
			px[__i_0__][4] = ar;
			cr = br - px[__i_0__][5];
			px[__i_0__][5] = br;
			ar = cr - px[__i_0__][6];
			px[__i_0__][6] = cr;
			br = ar - px[__i_0__][7];
			px[__i_0__][7] = ar;
			cr = br - px[__i_0__][8];
			px[__i_0__][8] = br;
			ar = cr - px[__i_0__][9];
			px[__i_0__][9] = cr;
			br = ar - px[__i_0__][10];
			px[__i_0__][10] = ar;
			cr = br - px[__i_0__][11];
			px[__i_0__][11] = br;
			px[__i_0__][13] = cr - px[__i_0__][12];
			px[__i_0__][12] = cr;
		}
	}

	i = __i_0__;
})

/**
 * @brief Run the benchmarks for lloops_c_kernels_line993_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(lloops_c_kernels_line993_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
