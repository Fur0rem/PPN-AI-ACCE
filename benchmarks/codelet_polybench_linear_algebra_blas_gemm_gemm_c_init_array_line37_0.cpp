/**
 * @file benchmarks/codelet_polybench_gemm_c_init_array_line37_0.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 0;
int ni = 1000;
int j = 0;
int nj = 1100;
double c[1000 + 0][1100 + 0];

BENCHMARK_FN(gemm_c_init_array_line37_loop_c_0_c, {
	int __i_0__ = i;
	int __j_1__ = j;

	{
		for (__i_0__ = 0; __i_0__ <= ni - 1; __i_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= nj - 1; __j_1__ += 1) {
				c[__i_0__][__j_1__] = ((double)(__i_0__ * __j_1__ % ni)) / ni;
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for gemm_c_init_array_line37_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(gemm_c_init_array_line37_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}