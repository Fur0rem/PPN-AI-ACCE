/**
 * @file codelets/codelet_polybench_cg_c_sparse_line757_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) cg_c_sparse_line757_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
constexpr int N = 14000;
typedef int boolean;
double x[N + 1];
boolean mark[N + 1];

BENCHMARK_FN(cg_c_sparse_line757_loop_c_0_c, {
	int __i_0__ = i;

	{
		for (__i_0__ = 1; __i_0__ <= N; __i_0__ += 1) {
			x[__i_0__] = 0.0;
			mark[__i_0__] = 0;
		}
	}

	i = __i_0__;
})

/**
 * @brief Run the benchmarks for cg_c_sparse_line757_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(cg_c_sparse_line757_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}