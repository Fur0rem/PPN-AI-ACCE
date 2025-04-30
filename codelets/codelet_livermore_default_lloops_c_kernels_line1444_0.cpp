/**
 * @file codelets/codelet_livermore_default_lloops_c_kernels_line1444_0.cpp
 * @brief Benchmark extracted from the Livermore loops codelet
 */

#include "bench_common.hpp"

long k = 0;
long i = 0;
long j = 0;
long inner_loops = 101;
double px[101][25] = {0};
double vy[25][101] = {0};
double cx[101][25] = {0};

BENCHMARK_FN(lloops_c_kernels_line1444_loop_c_0_c, {
	int __k_0__ = k;
	int __i_1__ = i;
	int __j_2__ = j;

	{
		for (__k_0__ = 0; __k_0__ < 25; __k_0__++) {
			for (__i_1__ = 0; __i_1__ < 25; __i_1__++) {
				for (__j_2__ = 0; __j_2__ < inner_loops; __j_2__++) {
					px[__j_2__][__i_1__] += vy[__k_0__][__i_1__] * cx[__j_2__][__k_0__];
				}
			}
		}
	}

	k = __k_0__;
	i = __i_1__;
	j = __j_2__;
})

/**
 * @brief Run the benchmarks for lloops_c_kernels_line1444_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(lloops_c_kernels_line1444_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
