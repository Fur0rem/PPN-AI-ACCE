/**
 * @file codelets/codelet_livermore_default_lloops_c_kernels_line1351_0.cpp
 * @brief Benchmark extracted from the Livermore loops codelet
 */

#include "bench_common.hpp"

long k = 0;
long kn = 6;
long j = 0;
long jnn = 100;
double zu[7][101] = {0};
double xtra[40] = {0};
double za[7][101] = {0};
double zz[7][101] = {0};
double zb[7][101] = {0};
double zv[7][101] = {0};
double zr[7][101] = {0};

OPTIMISE_SIZE
BENCHMARK_FN(lloops_c_kernels_line1351_loop_c_0_c, {
	int __k_0__ = k;
	int __j_1__ = j;

	{
		for (__k_0__ = 1; __k_0__ < kn; __k_0__++) {

			for (__j_1__ = 1; __j_1__ < jnn; __j_1__++) {
				zu[__k_0__][__j_1__] += xtra[32] * (za[__k_0__][__j_1__] * (zz[__k_0__][__j_1__] - zz[__k_0__][__j_1__ + 1]) -
													za[__k_0__][__j_1__ - 1] * (zz[__k_0__][__j_1__] - zz[__k_0__][__j_1__ - 1]) -
													zb[__k_0__][__j_1__] * (zz[__k_0__][__j_1__] - zz[__k_0__ - 1][__j_1__]) +
													zb[__k_0__ + 1][__j_1__] * (zz[__k_0__][__j_1__] - zz[__k_0__ + 1][__j_1__]));
				zv[__k_0__][__j_1__] += xtra[32] * (za[__k_0__][__j_1__] * (zr[__k_0__][__j_1__] - zr[__k_0__][__j_1__ + 1]) -
													za[__k_0__][__j_1__ - 1] * (zr[__k_0__][__j_1__] - zr[__k_0__][__j_1__ - 1]) -
													zb[__k_0__][__j_1__] * (zr[__k_0__][__j_1__] - zr[__k_0__ - 1][__j_1__]) +
													zb[__k_0__ + 1][__j_1__] * (zr[__k_0__][__j_1__] - zr[__k_0__ + 1][__j_1__]));
			}
		}
	}

	k = __k_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for lloops_c_kernels_line1351_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(lloops_c_kernels_line1351_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
