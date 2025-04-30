/**
 * @file codelets/codelet_polybench_nussinov_c_init_array_line42_0.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 2500;
int n = 2500;
int j = 0;
int table[2500 + 0][2500 + 0];

BENCHMARK_FN(nussinov_c_init_array_line42_loop_c_0_c, {
	int __i_0__ = i;
	int __j_1__ = j;

	{
		for (__i_0__ = 0; __i_0__ <= n - 1; __i_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= n - 1; __j_1__ += 1) {
				table[__i_0__][__j_1__] = 0;
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for nussinov_c_init_array_line42_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(nussinov_c_init_array_line42_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}