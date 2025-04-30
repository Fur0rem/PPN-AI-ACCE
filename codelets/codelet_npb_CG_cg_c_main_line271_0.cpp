/**
 * @file codelets/codelet_polybench_cg_c_main_line271_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) cg_c_main_line271_0 codelet
 */

#include "bench_common.hpp"

int j = 0;
int lastcol = 14000;
int firstcol = 1;
double z[14000 + 2 + 1];
double norm_temp11 = 0.000000;
double x[14000 + 2 + 1];
double norm_temp12 = 0.000000;

BENCHMARK_FN(cg_c_main_line271_loop_c_0_c, {
	int __j_0__ = j;
	double __zj_1__;

	{
		for (__j_0__ = 1; __j_0__ <= lastcol - firstcol + 1; __j_0__ += 1) {
			__zj_1__ = z[__j_0__];
			norm_temp11 = norm_temp11 + x[__j_0__] * __zj_1__;
			norm_temp12 = norm_temp12 + __zj_1__ * __zj_1__;
		}
	}

	j = __j_0__;
})

/**
 * @brief Run the benchmarks for cg_c_main_line271_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(cg_c_main_line271_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}