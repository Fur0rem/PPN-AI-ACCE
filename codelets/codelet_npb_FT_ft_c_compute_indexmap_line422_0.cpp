/**
 * @file codelets/codelet_polybench_ft_c_compute_indexmap_line422_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) ft_c_compute_indexmap_line422_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
double ex[6 * (256 * 256 / 4 + 256 * 256 / 4 + 128 * 128 / 4) + 1];

BENCHMARK_FN(ft_c_compute_indexmap_line422_loop_c_0_c, {
	int __i_0__ = i;

	{
		for (__i_0__ = 2; __i_0__ <= 221184; __i_0__ += 1) {
			ex[__i_0__] = ex[__i_0__ - 1] * ex[1];
		}
	}

	i = __i_0__;
})

/**
 * @brief Run the benchmarks for ft_c_compute_indexmap_line422_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(ft_c_compute_indexmap_line422_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}