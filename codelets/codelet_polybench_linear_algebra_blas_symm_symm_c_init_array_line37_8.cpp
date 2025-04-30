/**
 * @file codelets/codelet_polybench_symm_c_init_array_line37_8.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 0;
int m = 1000;
int j = 0;
int n = 1200;
double c[1000 + 0][1200 + 0];
double b[1000 + 0][1200 + 0];

OPTIMISE_SIZE
BENCHMARK_FN(symm_c_init_array_line37_loop_c_8_c, {
	int __i_0__ = i;
	int __j_1__ = j;

	{
		/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
		/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
		int _lu_fringe_11 = (((m - 1 + 1 - 0) % 1 == 0 ? (m - 1 + 1 - 0) / 1 : (m - 1 + 1 - 0) / 1 + 1)) % 4 == 0 ? 0 : 4;
		for (__i_0__ = 0; __i_0__ <= m - 1 - _lu_fringe_11; __i_0__ += 4) {
			for (__j_1__ = 0; __j_1__ <= n - 1; __j_1__ += 1) {
				c[__i_0__][__j_1__] = ((double)((__i_0__ + __j_1__) % 100)) / ((double)m);
				b[__i_0__][__j_1__] = ((double)((n + __i_0__ - __j_1__) % 100)) / ((double)m);
				c[__i_0__ + 1][__j_1__] = ((double)((__i_0__ + 1 + __j_1__) % 100)) / ((double)m);
				b[__i_0__ + 1][__j_1__] = ((double)((n + (__i_0__ + 1) - __j_1__) % 100)) / ((double)m);
				c[__i_0__ + 2][__j_1__] = ((double)((__i_0__ + 2 + __j_1__) % 100)) / ((double)m);
				b[__i_0__ + 2][__j_1__] = ((double)((n + (__i_0__ + 2) - __j_1__) % 100)) / ((double)m);
				c[__i_0__ + 3][__j_1__] = ((double)((__i_0__ + 3 + __j_1__) % 100)) / ((double)m);
				b[__i_0__ + 3][__j_1__] = ((double)((n + (__i_0__ + 3) - __j_1__) % 100)) / ((double)m);
			}
		}
		for (; __i_0__ <= m - 1; __i_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= n - 1; __j_1__ += 1) {
				c[__i_0__][__j_1__] = ((double)((__i_0__ + __j_1__) % 100)) / ((double)m);
				b[__i_0__][__j_1__] = ((double)((n + __i_0__ - __j_1__) % 100)) / ((double)m);
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for symm_c_init_array_line37_loop_c_8_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(symm_c_init_array_line37_loop_c_8_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}