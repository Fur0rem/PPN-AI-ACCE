/**
 * @file codelets/codelet_polybench_cg_c_conj_grad_line387_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) cg_c_conj_grad_line387_0 codelet
 */

#include "bench_common.hpp"

int j = 0;
int lastcol = 14000;
int firstcol = 1;
double rho = 0.000000;
double x[14001];

BENCHMARK_FN(cg_c_conj_grad_line387_loop_c_0_c, {
	int __j_0__ = j;
	double __xj_1__;

	{
		for (__j_0__ = 1; __j_0__ <= lastcol - firstcol + 1; __j_0__ += 1) {
			__xj_1__ = x[__j_0__];
			rho = rho + __xj_1__ * __xj_1__;
		}
	}

	j = __j_0__;
})

/**
 * @brief Run the benchmarks for cg_c_conj_grad_line387_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(cg_c_conj_grad_line387_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}