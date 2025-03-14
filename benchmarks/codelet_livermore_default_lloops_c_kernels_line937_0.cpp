/**
 * @file benchmarks/codelet_livermore_default_lloops_c_kernels_line937_0.cpp
 * @brief Benchmark extracted from the Livermore loops codelet
 */

#include "bench_common.hpp"

long kx = 0;
long ky = 0;
long inner_loops = 100;
double du1[101];
double u1[2][101][5];
long nl1 = 0;
double du2[101];
double u2[2][101][5];
double du3[101];
double u3[2][101][5];
long nl2 = 1;
double xtra[40];

BENCHMARK_FN(lloops_c_kernels_line937_loop_c_0_c, {
	int __kx_0__ = kx;
	int __ky_1__ = ky;

	{
		for (__kx_0__ = 1; __kx_0__ < 3; __kx_0__++) {
			for (__ky_1__ = 1; __ky_1__ < inner_loops; __ky_1__++) {
				du1[__ky_1__] = u1[nl1][__ky_1__ + 1][__kx_0__] - u1[nl1][__ky_1__ - 1][__kx_0__];
				du2[__ky_1__] = u2[nl1][__ky_1__ + 1][__kx_0__] - u2[nl1][__ky_1__ - 1][__kx_0__];
				du3[__ky_1__] = u3[nl1][__ky_1__ + 1][__kx_0__] - u3[nl1][__ky_1__ - 1][__kx_0__];
				u1[nl2][__ky_1__][__kx_0__] =
					u1[nl1][__ky_1__][__kx_0__] + xtra[1] * du1[__ky_1__] + xtra[2] * du2[__ky_1__] + xtra[3] * du3[__ky_1__] +
					xtra[34] * (u1[nl1][__ky_1__][__kx_0__ + 1] - 2.0 * u1[nl1][__ky_1__][__kx_0__] + u1[nl1][__ky_1__][__kx_0__ - 1]);
				u2[nl2][__ky_1__][__kx_0__] =
					u2[nl1][__ky_1__][__kx_0__] + xtra[4] * du1[__ky_1__] + xtra[5] * du2[__ky_1__] + xtra[6] * du3[__ky_1__] +
					xtra[34] * (u2[nl1][__ky_1__][__kx_0__ + 1] - 2.0 * u2[nl1][__ky_1__][__kx_0__] + u2[nl1][__ky_1__][__kx_0__ - 1]);
				u3[nl2][__ky_1__][__kx_0__] =
					u3[nl1][__ky_1__][__kx_0__] + xtra[7] * du1[__ky_1__] + xtra[8] * du2[__ky_1__] + xtra[9] * du3[__ky_1__] +
					xtra[34] * (u3[nl1][__ky_1__][__kx_0__ + 1] - 2.0 * u3[nl1][__ky_1__][__kx_0__] + u3[nl1][__ky_1__][__kx_0__ - 1]);
			}
		}
	}

	kx = __kx_0__;
	ky = __ky_1__;
})

/**
 * @brief Run the benchmarks for lloops_c_kernels_line937_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss)
					  .RUN(lloops_c_kernels_line937_loop_c_0_c)
					  .doNotOptimizeAway(du1)
					  .doNotOptimizeAway(u1)
					  .doNotOptimizeAway(du2)
					  .doNotOptimizeAway(u2)
					  .doNotOptimizeAway(du3)
					  .doNotOptimizeAway(u3)
					  .doNotOptimizeAway(xtra)
					  .results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
