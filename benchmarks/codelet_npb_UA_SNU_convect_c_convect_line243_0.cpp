/**
 * @file benchmarks/codelet_polybench_convect_c_convect_line243_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) convect_c_convect_line243_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
int nmor = 85456;
double tmort[92700];
double mormult[92700];

BENCHMARK_FN(convect_c_convect_line243_loop_c_0_c, {
	int __i_0__ = i;

	{
		for (__i_0__ = 0; __i_0__ <= nmor - 1; __i_0__ += 1) {
			tmort[__i_0__] = tmort[__i_0__] / mormult[__i_0__];
		}
	}

	i = __i_0__;
})

/**
 * @brief Run the benchmarks for convect_c_convect_line243_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(convect_c_convect_line243_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}