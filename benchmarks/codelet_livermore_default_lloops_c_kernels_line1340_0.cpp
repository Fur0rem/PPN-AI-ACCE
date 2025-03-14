/**
 * @file benchmarks/codelet_livermore_default_lloops_c_kernels_line1340_0.cpp
 * @brief Benchmark extracted from the Livermore loops codelet
 */

#include "bench_common.hpp"

long k = 0;
long kn = 6;
long j = 0;
long jnn = 100;
double za[7][101] = {0};
double zp[7][101] = {0};
double zq[7][101] = {0};
double zr[7][101] = {0};
double zm[7][101] = {0};
double zb[7][101] = {0};

BENCHMARK_FN(lloops_c_kernels_line1340_loop_c_0_c, {
	int __k_0__ = k;
	int __j_1__ = j;

	{
		for (__k_0__ = 1; __k_0__ < kn; __k_0__++) {
			for (__j_1__ = 1; __j_1__ < jnn; __j_1__++) {
				za[__k_0__][__j_1__] =
					(zp[__k_0__ + 1][__j_1__ - 1] + zq[__k_0__ + 1][__j_1__ - 1] - zp[__k_0__][__j_1__ - 1] - zq[__k_0__][__j_1__ - 1]) *
					(zr[__k_0__][__j_1__] + zr[__k_0__][__j_1__ - 1]) / (zm[__k_0__][__j_1__ - 1] + zm[__k_0__ + 1][__j_1__ - 1]);
				zb[__k_0__][__j_1__] = (zp[__k_0__][__j_1__ - 1] + zq[__k_0__][__j_1__ - 1] - zp[__k_0__][__j_1__] - zq[__k_0__][__j_1__]) *
									   (zr[__k_0__][__j_1__] + zr[__k_0__ - 1][__j_1__]) /
									   (zm[__k_0__][__j_1__] + zm[__k_0__][__j_1__ - 1]);
			}
		}
	}

	k = __k_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for lloops_c_kernels_line1340_main_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(lloops_c_kernels_line1340_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
