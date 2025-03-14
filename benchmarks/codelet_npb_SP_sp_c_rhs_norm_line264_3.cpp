/**
 * @file benchmarks/codelet_polybench_sp_c_rhs_norm_line264_3.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) sp_c_rhs_norm_line264_3 codelet
 */

#include "bench_common.hpp"

int i = 0;
int grid_points[3] = {64, 64, 64};
int j = 0;
int k = 0;
int m = 0;
double add = 0;
double rhs[5][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double rms[5];

OPTIMISE_SIZE
BENCHMARK_FN(sp_c_rhs_norm_line264_loop_c_3_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[1];
	int __arr_sub_2__ = grid_points[2];
	int __i_3__ = i;
	int __j_4__ = j;
	int __k_5__ = k;
	int __m_6__ = m;

	{
		for (__i_3__ = 0; __i_3__ <= __arr_sub_0__ - 2; __i_3__ += 1) {
			/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
			/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
			int _lu_fringe_3 =
				(((__arr_sub_1__ - 2 + 1 - 0) % 1 == 0 ? (__arr_sub_1__ - 2 + 1 - 0) / 1 : (__arr_sub_1__ - 2 + 1 - 0) / 1 + 1)) % 2 == 0
					? 0
					: 2;
			for (__j_4__ = 0; __j_4__ <= __arr_sub_1__ - 2 - _lu_fringe_3; __j_4__ += 2) {
				for (__k_5__ = 0; __k_5__ <= __arr_sub_2__ - 2; __k_5__ += 1) {
					for (__m_6__ = 0; __m_6__ <= 4; __m_6__ += 1) {
						add = rhs[__m_6__][__i_3__][__j_4__][__k_5__];
						rms[__m_6__] = rms[__m_6__] + add * add;
					}
					for (__m_6__ = 0; __m_6__ <= 4; __m_6__ += 1) {
						add = rhs[__m_6__][__i_3__][__j_4__ + 1][__k_5__];
						rms[__m_6__] = rms[__m_6__] + add * add;
					}
				}
			}
			for (; __j_4__ <= __arr_sub_1__ - 2; __j_4__ += 1) {
				for (__k_5__ = 0; __k_5__ <= __arr_sub_2__ - 2; __k_5__ += 1) {
					for (__m_6__ = 0; __m_6__ <= 4; __m_6__ += 1) {
						add = rhs[__m_6__][__i_3__][__j_4__][__k_5__];
						rms[__m_6__] = rms[__m_6__] + add * add;
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
 * @brief Run the benchmarks for sp_c_rhs_norm_line264_loop_c_3_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(sp_c_rhs_norm_line264_loop_c_3_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}