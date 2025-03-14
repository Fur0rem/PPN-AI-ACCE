/**
 * @file benchmarks/codelet_polybench_bt_c_rhs_norm_line261_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) bt_c_rhs_norm_line261_0 codelet
 */

#include "bench_common.hpp"

int i = 1835557742;
int grid_points[3] = {64, 64, 64};
int j = 1700933481;
int k = 1752837989;
int m = 5;
double add = 0.000000;
double rhs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1][5];
double rms[5];

BENCHMARK_FN(bt_c_rhs_norm_line261_loop_c_0_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[1];
	int __arr_sub_2__ = grid_points[2];
	int __i_3__ = i;
	int __j_4__ = j;
	int __k_5__ = k;
	int __m_6__ = m;

	{
		for (__i_3__ = 1; __i_3__ <= __arr_sub_0__ - 1 - 1; __i_3__ += 1) {
			for (__j_4__ = 1; __j_4__ <= __arr_sub_1__ - 1 - 1; __j_4__ += 1) {
				for (__k_5__ = 1; __k_5__ <= __arr_sub_2__ - 1 - 1; __k_5__ += 1) {
					for (__m_6__ = 0; __m_6__ <= 4; __m_6__ += 1) {
						add = rhs[__i_3__][__j_4__][__k_5__][__m_6__];
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
 * @brief Run the benchmarks for bt_c_rhs_norm_line261_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(bt_c_rhs_norm_line261_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}