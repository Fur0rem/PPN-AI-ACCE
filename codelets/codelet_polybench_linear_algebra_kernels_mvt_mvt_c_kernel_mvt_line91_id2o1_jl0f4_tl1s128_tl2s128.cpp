/**
 * @file codelets/codelet_polybench_mvt_c_kernel_mvt_line91_id2o1_jl0f4_tl1s128_tl2s128.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 2000;
int n = 2000;
int j = 2000;
double x2[2000 + 0];
double A[2000 + 0][2000 + 0];
double y_2[2000 + 0];

OPTIMISE_SIZE
BENCHMARK_FN(mvt_c_kernel_mvt_line91_loop_c_44_c, {
	int __i_0__ = i;
	int __j_1__ = j;

	{
		/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
		/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
		int _lu_fringe_46 = (((n - 1 + 1 - 0) % 1 == 0 ? (n - 1 + 1 - 0) / 1 : (n - 1 + 1 - 0) / 1 + 1)) % 4 == 0 ? 0 : 4;
		int _lt_var___j_1__;
		for (_lt_var___j_1__ = 0; _lt_var___j_1__ <= n - 1 - _lu_fringe_46; _lt_var___j_1__ += 512) {
			int _lt_var___i_0__;
			for (_lt_var___i_0__ = 0; _lt_var___i_0__ <= n - 1; _lt_var___i_0__ += 128) {
				for (__j_1__ = _lt_var___j_1__;
					 __j_1__ <=
					 (((n - 1 - _lu_fringe_46 < (_lt_var___j_1__ + 128 - 1)) ? (n - 1 - _lu_fringe_46) : (_lt_var___j_1__ + 128 - 1)));
					 __j_1__ += 4) {
					for (__i_0__ = _lt_var___i_0__;
						 __i_0__ <= (((n - 1 < (_lt_var___i_0__ + 128 - 1)) ? (n - 1) : (_lt_var___i_0__ + 128 - 1)));
						 __i_0__ += 1) {
						x2[__i_0__] = x2[__i_0__] + A[__j_1__][__i_0__] * y_2[__j_1__];
						x2[__i_0__] = x2[__i_0__] + A[__j_1__ + 1][__i_0__] * y_2[__j_1__ + 1];
						x2[__i_0__] = x2[__i_0__] + A[__j_1__ + 2][__i_0__] * y_2[__j_1__ + 2];
						x2[__i_0__] = x2[__i_0__] + A[__j_1__ + 3][__i_0__] * y_2[__j_1__ + 3];
					}
				}
			}
		}
		for (; __j_1__ <= n - 1; __j_1__ += 1) {
			for (__i_0__ = 0; __i_0__ <= n - 1; __i_0__ += 1) {
				x2[__i_0__] = x2[__i_0__] + A[__j_1__][__i_0__] * y_2[__j_1__];
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for mvt_c_kernel_mvt_line91_loop_c_44_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(3, oss).RUN(mvt_c_kernel_mvt_line91_loop_c_44_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}