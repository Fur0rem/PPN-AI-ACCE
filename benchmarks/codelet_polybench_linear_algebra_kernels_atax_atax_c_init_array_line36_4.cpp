/**
 * @file benchmarks/codelet_polybench_atax_c_init_array_line36_4.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 2100;
int m = 1900;
int j = 0;
int n = 2100;
double a[1900 + 0][2100 + 0];

BENCHMARK_FN(atax_c_init_array_line36_loop_c_4_c, {
	int __i_0__ = i;
	int __j_1__ = j;

	{
		/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
		/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
		int _lu_fringe_4 = (((m - 1 + 1 - 0) % 1 == 0 ? (m - 1 + 1 - 0) / 1 : (m - 1 + 1 - 0) / 1 + 1)) % 2 == 0 ? 0 : 2;
		for (__i_0__ = 0; __i_0__ <= m - 1 - _lu_fringe_4; __i_0__ += 2) {
			for (__j_1__ = 0; __j_1__ <= n - 1; __j_1__ += 1) {
				a[__i_0__][__j_1__] = ((double)((__i_0__ + __j_1__) % n)) / ((double)(5 * m));
				a[__i_0__ + 1][__j_1__] = ((double)((__i_0__ + 1 + __j_1__) % n)) / ((double)(5 * m));
			}
		}
		for (; __i_0__ <= m - 1; __i_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= n - 1; __j_1__ += 1) {
				a[__i_0__][__j_1__] = ((double)((__i_0__ + __j_1__) % n)) / ((double)(5 * m));
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for atax_c_init_array_line36_loop_c_4_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(atax_c_init_array_line36_loop_c_4_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}