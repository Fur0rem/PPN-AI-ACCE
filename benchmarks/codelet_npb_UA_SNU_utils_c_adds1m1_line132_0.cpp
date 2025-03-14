/**
 * @file benchmarks/codelet_polybench_utils_c_adds1m1_line132_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) utils_c_adds1m1_line132_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
constexpr int N = 250375;
double c1 = 0.198297;
double a[N];
double b[N];

BENCHMARK_FN(utils_c_adds1m1_line132_loop_c_0_c, {
	int __i_0__ = i;

	{
		for (__i_0__ = 0; __i_0__ <= N - 1; __i_0__ += 1) {
			a[__i_0__] = c1 * a[__i_0__] + b[__i_0__];
		}
	}

	i = __i_0__;
})

BENCHMARK_FN(init_arrays, {
	for (int i = 0; i < N; i++) {
		a[i] = 42.0;
		b[i] = 99.0;
	}
})

/**
 * @brief Run the benchmarks for utils_c_adds1m1_line132_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(100, oss).RUN(init_arrays).RUN(utils_c_adds1m1_line132_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}