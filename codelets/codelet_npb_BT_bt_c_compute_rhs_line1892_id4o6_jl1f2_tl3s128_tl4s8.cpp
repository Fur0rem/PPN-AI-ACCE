/**
 * @file codelets/codelet_polybench_bt_c_compute_rhs_line1892_id4o6_jl1f2_tl3s128_tl4s8.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) bt_c_compute_rhs_line1892_id4o6_jl1f2_tl3s128_tl4s8 codelet
 */

#include "bench_common.hpp"

int i = 2;
int grid_points[3] = {64, 64, 64};
int j = 63;
int k = 63;
int m = 5;
double rhs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1][5];
double dssp = 0.250000;
double u[(64 + 1) / 2 * 2 + 1][(64 + 1) / 2 * 2 + 1][(64 + 1) / 2 * 2 + 1][5];

OPTIMISE_SIZE
BENCHMARK_FN(bt_c_compute_rhs_line1892_loop_c_361_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[1];
	int __arr_sub_2__ = grid_points[2];
	int __i_3__ = i;
	int __j_4__ = j;
	int __k_5__ = k;
	int __m_6__ = m;

	{
		int _lt_var___k_5__;
		NO_VECTORISE_NO_UNROLL
		for (_lt_var___k_5__ = 1; _lt_var___k_5__ <= __arr_sub_2__ - 1 - 1; _lt_var___k_5__ += 128) {
			int _lt_var___m_6__;
			NO_VECTORISE_NO_UNROLL
			for (_lt_var___m_6__ = 0; _lt_var___m_6__ <= 4; _lt_var___m_6__ += 8) {
				NO_VECTORISE_NO_UNROLL
				for (__j_4__ = 1; __j_4__ <= __arr_sub_1__ - 1 - 1; __j_4__ += 1) {
					/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
					/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
					int _lu_fringe_537 = (((__arr_sub_0__ - 3 - 1 + 1 - 3) % 1 == 0 ? (__arr_sub_0__ - 3 - 1 + 1 - 3) / 1
																					: (__arr_sub_0__ - 3 - 1 + 1 - 3) / 1 + 1)) %
													 2 ==
												 0
											 ? 0
											 : 2;
					NO_VECTORISE_NO_UNROLL
					for (__i_3__ = 3; __i_3__ <= __arr_sub_0__ - 3 - 1 - _lu_fringe_537; __i_3__ += 2) {
						NO_VECTORISE_NO_UNROLL
						for (__k_5__ = _lt_var___k_5__;
							 __k_5__ <= (((__arr_sub_2__ - 1 - 1 < (_lt_var___k_5__ + 128 - 1)) ? (__arr_sub_2__ - 1 - 1)
																								: (_lt_var___k_5__ + 128 - 1)));
							 __k_5__ += 1) {
							NO_VECTORISE_NO_UNROLL
							for (__m_6__ = _lt_var___m_6__; __m_6__ <= (((4 < (_lt_var___m_6__ + 8 - 1)) ? 4 : (_lt_var___m_6__ + 8 - 1)));
								 __m_6__ += 1) {
								rhs[__i_3__][__j_4__][__k_5__][__m_6__] =
									rhs[__i_3__][__j_4__][__k_5__][__m_6__] -
									dssp * (u[__i_3__ - 2][__j_4__][__k_5__][__m_6__] - 4.0 * u[__i_3__ - 1][__j_4__][__k_5__][__m_6__] +
											6.0 * u[__i_3__][__j_4__][__k_5__][__m_6__] - 4.0 * u[__i_3__ + 1][__j_4__][__k_5__][__m_6__] +
											u[__i_3__ + 2][__j_4__][__k_5__][__m_6__]);
							}
							NO_VECTORISE_NO_UNROLL
							for (__m_6__ = 0; __m_6__ <= 4; __m_6__ += 1) {
								rhs[__i_3__ + 1][__j_4__][__k_5__][__m_6__] = rhs[__i_3__ + 1][__j_4__][__k_5__][__m_6__] -
																			  dssp * (u[__i_3__ + 1 - 2][__j_4__][__k_5__][__m_6__] -
																					  4.0 * u[__i_3__ + 1 - 1][__j_4__][__k_5__][__m_6__] +
																					  6.0 * u[__i_3__ + 1][__j_4__][__k_5__][__m_6__] -
																					  4.0 * u[__i_3__ + 1 + 1][__j_4__][__k_5__][__m_6__] +
																					  u[__i_3__ + 1 + 2][__j_4__][__k_5__][__m_6__]);
							}
						}
					}
					NO_VECTORISE_NO_UNROLL
					for (; __i_3__ <= __arr_sub_0__ - 3 - 1; __i_3__ += 1) {
						NO_VECTORISE_NO_UNROLL
						for (__k_5__ = 1; __k_5__ <= __arr_sub_2__ - 1 - 1; __k_5__ += 1) {
							NO_VECTORISE_NO_UNROLL
							for (__m_6__ = 0; __m_6__ <= 4; __m_6__ += 1) {
								rhs[__i_3__][__j_4__][__k_5__][__m_6__] =
									rhs[__i_3__][__j_4__][__k_5__][__m_6__] -
									dssp * (u[__i_3__ - 2][__j_4__][__k_5__][__m_6__] - 4.0 * u[__i_3__ - 1][__j_4__][__k_5__][__m_6__] +
											6.0 * u[__i_3__][__j_4__][__k_5__][__m_6__] - 4.0 * u[__i_3__ + 1][__j_4__][__k_5__][__m_6__] +
											u[__i_3__ + 2][__j_4__][__k_5__][__m_6__]);
							}
						}
					}
				}
			}
		}
	}

	i = __i_3__;
	j = __j_4__;
	k = __k_5__;
	m = __m_6__;
})

/**
 * @brief Run the benchmarks for bt_c_compute_rhs_line1892_loop_c_361_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(bt_c_compute_rhs_line1892_loop_c_361_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}