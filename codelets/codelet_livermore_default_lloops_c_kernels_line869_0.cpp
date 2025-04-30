/**
 * @file codelets/codelet_livermore_default_lloops_c_kernels_line869_0.cpp
 * @brief Benchmark extracted from the Livermore loops codelet
 */

#include "bench_common.hpp"

long i = 0;
long inner_loops = 1001;
double x[1001];
double z[1001];
double y[1001];

BENCHMARK_FN(lloops_c_kernels_line869_loop_c_0_c, {
	int __i_0__ = i;

	{
		for (__i_0__ = 1; __i_0__ < inner_loops; __i_0__++) {
			x[__i_0__] = z[__i_0__] * (y[__i_0__] - x[__i_0__ - 1]);
		}
	}

	i = __i_0__;
})

/**
 * @brief Run the benchmarks for lloops_c_kernels_line869_main_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss)
					  .RUN(lloops_c_kernels_line869_loop_c_0_c)
					  .doNotOptimizeAway(x)
					  .doNotOptimizeAway(z)
					  .doNotOptimizeAway(y)
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
