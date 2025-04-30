/**
 * @file codelets/codelet_polybench_bt_c_exact_rhs_line300_358.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) bt_c_exact_rhs_line300_358 codelet
 */

#include "bench_common.hpp"

int i = 0;
int grid_points[3] = {64, 64, 64};
int j = 0;
int k = 0;
int m = 0;
double forcing[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1][5 + 1];

BENCHMARK_FN(bt_c_exact_rhs_line300_loop_c_358_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[1];
	int __arr_sub_2__ = grid_points[2];
	int __i_3__ = i;
	int __j_4__ = j;
	int __k_5__ = k;
	int __m_6__ = m;

	{
		/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
		/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
		int _lu_fringe_526 =
			(((__arr_sub_1__ - 1 + 1 - 0) % 1 == 0 ? (__arr_sub_1__ - 1 + 1 - 0) / 1 : (__arr_sub_1__ - 1 + 1 - 0) / 1 + 1)) % 4 == 0 ? 0
																																	  : 4;
		for (__j_4__ = 0; __j_4__ <= __arr_sub_1__ - 1 - _lu_fringe_526; __j_4__ += 4) {
			for (__i_3__ = 0; __i_3__ <= __arr_sub_0__ - 1; __i_3__ += 1) {
				for (__k_5__ = 0; __k_5__ <= __arr_sub_2__ - 1; __k_5__ += 1) {
					for (__m_6__ = 0; __m_6__ <= 4; __m_6__ += 1) {
						forcing[__i_3__][__j_4__][__k_5__][__m_6__] = 0.0;
					}
				}
				for (__k_5__ = 0; __k_5__ <= __arr_sub_2__ - 1; __k_5__ += 1) {
					for (__m_6__ = 0; __m_6__ <= 4; __m_6__ += 1) {
						forcing[__i_3__][__j_4__ + 1][__k_5__][__m_6__] = 0.0;
					}
				}
				for (__k_5__ = 0; __k_5__ <= __arr_sub_2__ - 1; __k_5__ += 1) {
					for (__m_6__ = 0; __m_6__ <= 4; __m_6__ += 1) {
						forcing[__i_3__][__j_4__ + 2][__k_5__][__m_6__] = 0.0;
					}
				}
				for (__k_5__ = 0; __k_5__ <= __arr_sub_2__ - 1; __k_5__ += 1) {
					for (__m_6__ = 0; __m_6__ <= 4; __m_6__ += 1) {
						forcing[__i_3__][__j_4__ + 3][__k_5__][__m_6__] = 0.0;
					}
				}
			}
		}
		for (; __j_4__ <= __arr_sub_1__ - 1; __j_4__ += 1) {
			for (__i_3__ = 0; __i_3__ <= __arr_sub_0__ - 1; __i_3__ += 1) {
				for (__k_5__ = 0; __k_5__ <= __arr_sub_2__ - 1; __k_5__ += 1) {
					for (__m_6__ = 0; __m_6__ <= 4; __m_6__ += 1) {
						forcing[__i_3__][__j_4__][__k_5__][__m_6__] = 0.0;
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
 * @brief Run the benchmarks for bt_c_exact_rhs_line300_loop_c_358_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(bt_c_exact_rhs_line300_loop_c_358_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}