/**
 * @file benchmarks/codelet_livermore_default_lloops_c_kernels_line1113_0.cpp
 * @brief Benchmark extracted from the Livermore loops codelet
 */

#include "bench_common.hpp"

long k = 0;
long inner_loops = 1001;
double vx[1001] = {0};
double ex1[1001] = {0};
double xx[1001] = {0};
double xi[1001] = {0};
double dex1[1001] = {0};
double xtra[40] = {0};
long ir[1001] = {0};
double rx[1001] = {0};

BENCHMARK_FN(lloops_c_kernels_line1113_loop_c_0_c, {
	int __k_0__ = k;

	{
		for (__k_0__ = 0; __k_0__ < inner_loops; __k_0__++) {
			vx[__k_0__] = vx[__k_0__] + ex1[__k_0__] + (xx[__k_0__] - xi[__k_0__]) * dex1[__k_0__];
			xx[__k_0__] = xx[__k_0__] + vx[__k_0__] + xtra[27];
			ir[__k_0__] = ((long)xx[__k_0__]);
			rx[__k_0__] = xx[__k_0__] - ir[__k_0__];
			ir[__k_0__] = (ir[__k_0__] & (2048 - 1)) + 1;
			xx[__k_0__] = rx[__k_0__] + ir[__k_0__];
		}
	}

	k = __k_0__;
})

/**
 * @brief Run the benchmarks for lloops_c_kernels_line1113_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss)
					  .RUN(lloops_c_kernels_line1113_loop_c_0_c)
					  .doNotOptimizeAway(vx)
					  .doNotOptimizeAway(ex1)
					  .doNotOptimizeAway(xx)
					  .doNotOptimizeAway(xi)
					  .doNotOptimizeAway(dex1)
					  .doNotOptimizeAway(xtra)
					  .doNotOptimizeAway(ir)
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
