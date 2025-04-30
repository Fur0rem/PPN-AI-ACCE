/**
 * @file codelets/codelet_polybench_deriche_c_init_array_line35_jl0f2_tl1s256_tl2s256.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 0;
int w = 4096;
int j = 0;
int h = 2160;
float img_in[4096 + 0][2160 + 0];

OPTIMISE_SIZE
BENCHMARK_FN(deriche_c_init_array_line35_loop_c_4_c, {
	int __i_0__ = i;
	int __j_1__ = j;

	{
		/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
		/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
		int _lu_fringe_4 = (((w - 1 + 1 - 0) % 1 == 0 ? (w - 1 + 1 - 0) / 1 : (w - 1 + 1 - 0) / 1 + 1)) % 2 == 0 ? 0 : 2;
		int _lt_var___i_0__;
		for (_lt_var___i_0__ = 0; _lt_var___i_0__ <= w - 1 - _lu_fringe_4; _lt_var___i_0__ += 512) {
			int _lt_var___j_1__;
			for (_lt_var___j_1__ = 0; _lt_var___j_1__ <= h - 1; _lt_var___j_1__ += 256) {
				for (__i_0__ = _lt_var___i_0__;
					 __i_0__ <=
					 (((w - 1 - _lu_fringe_4 < (_lt_var___i_0__ + 256 - 1)) ? (w - 1 - _lu_fringe_4) : (_lt_var___i_0__ + 256 - 1)));
					 __i_0__ += 2) {
					for (__j_1__ = _lt_var___j_1__;
						 __j_1__ <= (((h - 1 < (_lt_var___j_1__ + 256 - 1)) ? (h - 1) : (_lt_var___j_1__ + 256 - 1)));
						 __j_1__ += 1) {
						img_in[__i_0__][__j_1__] = ((float)((313 * __i_0__ + 991 * __j_1__) % 65536)) / 65535.0f;
						img_in[__i_0__ + 1][__j_1__] = ((float)((313 * (__i_0__ + 1) + 991 * __j_1__) % 65536)) / 65535.0f;
					}
				}
			}
		}
		for (; __i_0__ <= w - 1; __i_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= h - 1; __j_1__ += 1) {
				img_in[__i_0__][__j_1__] = ((float)((313 * __i_0__ + 991 * __j_1__) % 65536)) / 65535.0f;
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for deriche_c_init_array_line35_loop_c_4_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(deriche_c_init_array_line35_loop_c_4_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}