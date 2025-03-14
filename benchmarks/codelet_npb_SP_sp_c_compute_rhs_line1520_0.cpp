/**
 * @file benchmarks/codelet_polybench_sp_c_compute_rhs_line1520_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) sp_c_compute_rhs_line1520_0 codelet
 */

#include "bench_common.hpp"

int m = 0;
int i = 0;
int grid_points[3] = {64, 64, 64};
int k = 0;
double rhs[5][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
int j = 2;
double dssp = 0.250000;
double u[5][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];

BENCHMARK_FN(sp_c_compute_rhs_line1520_loop_c_0_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[2];
	int __m_2__ = m;
	int __i_3__ = i;
	int __k_4__ = k;

	{
		for (__m_2__ = 0; __m_2__ <= 4; __m_2__ += 1) {
			for (__i_3__ = 1; __i_3__ <= __arr_sub_0__ - 2; __i_3__ += 1) {
				for (__k_4__ = 1; __k_4__ <= __arr_sub_1__ - 2; __k_4__ += 1) {
					rhs[__m_2__][__i_3__][j][__k_4__] =
						rhs[__m_2__][__i_3__][j][__k_4__] -
						dssp * (-4.0 * u[__m_2__][__i_3__][j - 1][__k_4__] + 6.0 * u[__m_2__][__i_3__][j][__k_4__] -
								4.0 * u[__m_2__][__i_3__][j + 1][__k_4__] + u[__m_2__][__i_3__][j + 2][__k_4__]);
				}
			}
		}
	}

	m = __m_2__;
	i = __i_3__;
	k = __k_4__;
})

/**
 * @brief Run the benchmarks for sp_c_compute_rhs_line1520_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(sp_c_compute_rhs_line1520_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}