/**
 * @file benchmarks/codelet_polybench_bt_c_compute_rhs_line2171_1088.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) bt_c_compute_rhs_line2171_1088 codelet
 */

#include "bench_common.hpp"

int j = 63;
int grid_points[3] = {64, 64, 64};
int k = 62;
int m = 5;
int i = 63;
double rhs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1][5];
double dt = 0.000800;

BENCHMARK_FN(bt_c_compute_rhs_line2171_loop_c_1088_c, {
	int __arr_sub_0__ = grid_points[1];
	int __arr_sub_1__ = grid_points[2];
	int __arr_sub_2__ = grid_points[0];
	int __j_3__ = j;
	int __k_4__ = k;
	int __m_5__ = m;
	int __i_6__ = i;

	{
		for (__i_6__ = 1; __i_6__ <= __arr_sub_2__ - 1 - 1; __i_6__ += 1) {
			for (__j_3__ = 1; __j_3__ <= __arr_sub_0__ - 1 - 1; __j_3__ += 1) {
				for (__k_4__ = 1; __k_4__ <= __arr_sub_1__ - 1 - 1; __k_4__ += 1) {
					for (__m_5__ = 0; __m_5__ <= 4; __m_5__ += 1) {
						rhs[__i_6__][__j_3__][__k_4__][__m_5__] = rhs[__i_6__][__j_3__][__k_4__][__m_5__] * dt;
					}
				}
			}
		}
	}

	j = __j_3__;
	k = __k_4__;
	m = __m_5__;
	i = __i_6__;
})

/**
 * @brief Run the benchmarks for bt_c_compute_rhs_line2171_loop_c_1088_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(bt_c_compute_rhs_line2171_loop_c_1088_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}