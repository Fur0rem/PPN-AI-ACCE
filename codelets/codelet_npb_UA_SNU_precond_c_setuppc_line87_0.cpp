/**
 * @file codelets/codelet_polybench_precond_c_setuppc_line87_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) precond_c_setuppc_line87_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
int nmor = 85456;
double dpcmor[92700];

BENCHMARK_FN(precond_c_setuppc_line87_loop_c_0_c, {
	int __i_0__ = i;

	{
		for (__i_0__ = 0; __i_0__ <= nmor - 1; __i_0__ += 1) {
			dpcmor[__i_0__] = 1.0 / dpcmor[__i_0__];
		}
	}

	i = __i_0__;
})

/**
 * @brief Run the benchmarks for precond_c_setuppc_line87_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(precond_c_setuppc_line87_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}