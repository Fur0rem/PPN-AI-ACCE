/**
 * @file benchmarks/codelet_polybench_gramschmidt_c_init_array_line33_1.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 0;
int m = 1000;
int j = 0;
int n = 1200;
double A[1000 + 0][1200 + 0];
double Q[1000 + 0][1200 + 0];

BENCHMARK_FN(gramschmidt_c_init_array_line33_loop_c_1_c, {
	int __i_0__ = i;
	int __j_1__ = j;

	{
		for (__i_0__ = 0; __i_0__ <= m - 1; __i_0__ += 1) {
			/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
			/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
			int _lu_fringe_1 = (((n - 1 + 1 - 0) % 1 == 0 ? (n - 1 + 1 - 0) / 1 : (n - 1 + 1 - 0) / 1 + 1)) % 2 == 0 ? 0 : 2;
			for (__j_1__ = 0; __j_1__ <= n - 1 - _lu_fringe_1; __j_1__ += 2) {
				A[__i_0__][__j_1__] = ((double)(__i_0__ * __j_1__ % m)) / ((double)m) * ((double)100) + ((double)10);
				Q[__i_0__][__j_1__] = 0.0;
				{
					A[__i_0__][__j_1__ + 1] = ((double)(__i_0__ * (__j_1__ + 1) % m)) / ((double)m) * ((double)100) + ((double)10);
					Q[__i_0__][__j_1__ + 1] = 0.0;
				}
			}
			for (; __j_1__ <= n - 1; __j_1__ += 1) {
				A[__i_0__][__j_1__] = ((double)(__i_0__ * __j_1__ % m)) / ((double)m) * ((double)100) + ((double)10);
				Q[__i_0__][__j_1__] = 0.0;
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for gramschmidt_c_init_array_line33_loop_c_1_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(gramschmidt_c_init_array_line33_loop_c_1_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}