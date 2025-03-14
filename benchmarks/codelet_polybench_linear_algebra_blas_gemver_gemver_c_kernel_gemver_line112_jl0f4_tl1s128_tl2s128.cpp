/**
 * @file benchmarks/codelet_polybench_gemver_c_kernel_gemver_line112_jl0f4_tl1s128_tl2s128.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 2000;
int n = 2000;
int j = 2000;
double w[2000 + 0];
double alpha = 1.500000;
double a[2000 + 0][2000 + 0];
double x[2000 + 0];

BENCHMARK_FN(gemver_c_kernel_gemver_line112_loop_c_8_c, {
	int __i_0__ = i;
	int __j_1__ = j;

	{
		/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
		/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
		int _lu_fringe_11 = (((n - 1 + 1 - 0) % 1 == 0 ? (n - 1 + 1 - 0) / 1 : (n - 1 + 1 - 0) / 1 + 1)) % 4 == 0 ? 0 : 4;
		int _lt_var___i_0__;
		for (_lt_var___i_0__ = 0; _lt_var___i_0__ <= n - 1 - _lu_fringe_11; _lt_var___i_0__ += 512) {
			int _lt_var___j_1__;
			for (_lt_var___j_1__ = 0; _lt_var___j_1__ <= n - 1; _lt_var___j_1__ += 128) {
				for (__i_0__ = _lt_var___i_0__;
					 __i_0__ <=
					 (((n - 1 - _lu_fringe_11 < (_lt_var___i_0__ + 128 - 1)) ? (n - 1 - _lu_fringe_11) : (_lt_var___i_0__ + 128 - 1)));
					 __i_0__ += 4) {
					for (__j_1__ = _lt_var___j_1__;
						 __j_1__ <= (((n - 1 < (_lt_var___j_1__ + 128 - 1)) ? (n - 1) : (_lt_var___j_1__ + 128 - 1)));
						 __j_1__ += 1) {
						w[__i_0__] = w[__i_0__] + alpha * a[__i_0__][__j_1__] * x[__j_1__];
						w[__i_0__ + 1] = w[__i_0__ + 1] + alpha * a[__i_0__ + 1][__j_1__] * x[__j_1__];
						w[__i_0__ + 2] = w[__i_0__ + 2] + alpha * a[__i_0__ + 2][__j_1__] * x[__j_1__];
						w[__i_0__ + 3] = w[__i_0__ + 3] + alpha * a[__i_0__ + 3][__j_1__] * x[__j_1__];
					}
				}
			}
		}
		for (; __i_0__ <= n - 1; __i_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= n - 1; __j_1__ += 1) {
				w[__i_0__] = w[__i_0__] + alpha * a[__i_0__][__j_1__] * x[__j_1__];
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for gemver_c_kernel_gemver_line112_loop_c_8_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(3, oss).RUN(gemver_c_kernel_gemver_line112_loop_c_8_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}