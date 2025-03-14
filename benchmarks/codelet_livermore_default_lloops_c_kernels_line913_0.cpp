/**
 * @file benchmarks/codelet_livermore_default_lloops_c_kernels_line913_0.cpp
 * @brief Benchmark extracted from the Livermore loops codelet
 */

#include "bench_common.hpp"

long k = 0;
long inner_loops = 995;
double x[1001];
double u[1001];
double xtra[40];
double z[1001];
double y[1001];

BENCHMARK_FN(lloops_c_kernels_line913_loop_c_0_c, {
	int __k_0__ = k;

	{
		for (__k_0__ = 0; __k_0__ < inner_loops; __k_0__++) {
			x[__k_0__] = u[__k_0__] + xtra[30] * (z[__k_0__] + xtra[30] * y[__k_0__]) +
						 xtra[36] * (u[__k_0__ + 3] + xtra[30] * (u[__k_0__ + 2] + xtra[30] * u[__k_0__ + 1]) +
									 xtra[36] * (u[__k_0__ + 6] + xtra[28] * (u[__k_0__ + 5] + xtra[28] * u[__k_0__ + 4])));
		}
	}

	k = __k_0__;
})

/**
 * @brief Run the benchmarks for lloops_c_kernels_line913_main_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(lloops_c_kernels_line913_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
