/**
 * @file benchmarks/codelet_polybench_covariance_c_init_array_line34_0.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 0;
int j = 0;
double data[1400 + 0][1200 + 0];

BENCHMARK_FN(covariance_c_init_array_line34_loop_c_0_c, {
	int __i_0__ = i;
	int __j_1__ = j;

	{
		for (__i_0__ = 0; __i_0__ <= 1399; __i_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= 1199; __j_1__ += 1) {
				data[__i_0__][__j_1__] = ((double)__i_0__) * __j_1__ / 1200;
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for covariance_c_init_array_line34_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(covariance_c_init_array_line34_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}