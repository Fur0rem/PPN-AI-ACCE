/**
 * @file codelets/codelet_polybench_cg_c_main_line227_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) cg_c_main_line227_0 codelet
 */

#include "bench_common.hpp"

int j = 0;
int lastcol = 14000;
int firstcol = 1;
double x[14000 + 2 + 1];
double norm_temp12 = 0.026761;
double z[14000 + 2 + 1];

BENCHMARK_FN(cg_c_main_line227_loop_c_0_c, {
	int __j_0__ = j;

	{
		for (__j_0__ = 1; __j_0__ <= lastcol - firstcol + 1; __j_0__ += 1) {
			x[__j_0__] = norm_temp12 * z[__j_0__];
		}
	}

	j = __j_0__;
})

/**
 * @brief Run the benchmarks for cg_c_main_line227_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(cg_c_main_line227_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}