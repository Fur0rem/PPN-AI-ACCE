/**
 * @file codelets/codelet_polybench_bt_c_compute_rhs_line2027_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) bt_c_compute_rhs_line2027_0 codelet
 */

#include "bench_common.hpp"

int i = 63;
int grid_points[3] = {64, 64, 64};
int k = 63;
int m = 5;
double rhs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1][5];
int j = 61;
double dssp = 0.250000;
double u[(64 + 1) / 2 * 2 + 1][(64 + 1) / 2 * 2 + 1][(64 + 1) / 2 * 2 + 1][5];

OPTIMISE_SIZE
BENCHMARK_FN(bt_c_compute_rhs_line2027_loop_c_0_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[2];
	int __i_2__ = i;
	int __k_3__ = k;
	int __m_4__ = m;

	{
		for (__i_2__ = 1; __i_2__ <= __arr_sub_0__ - 1 - 1; __i_2__ += 1) {
			for (__k_3__ = 1; __k_3__ <= __arr_sub_1__ - 1 - 1; __k_3__ += 1) {
				for (__m_4__ = 0; __m_4__ <= 4; __m_4__ += 1) {
					rhs[__i_2__][j][__k_3__][__m_4__] =
						rhs[__i_2__][j][__k_3__][__m_4__] -
						dssp * (u[__i_2__][j - 2][__k_3__][__m_4__] - 4.0 * u[__i_2__][j - 1][__k_3__][__m_4__] +
								6.0 * u[__i_2__][j][__k_3__][__m_4__] - 4.0 * u[__i_2__][j + 1][__k_3__][__m_4__]);
				}
			}
		}
	}

	i = __i_2__;
	k = __k_3__;
	m = __m_4__;
})

/**
 * @brief Run the benchmarks for bt_c_compute_rhs_line2027_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(bt_c_compute_rhs_line2027_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}