/**
 * @file benchmarks/codelet_polybench_sp_c_compute_rhs_line1630_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) sp_c_compute_rhs_line1630_0 codelet
 */

#include "bench_common.hpp"

int m = 0;
int i = 0;
int grid_points[3] = {64, 64, 64};
int j = 0;
double rhs[5][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
int k = 1;
double dssp = 0.250000;
double u[5][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];

BENCHMARK_FN(sp_c_compute_rhs_line1630_loop_c_0_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[1];
	int __m_2__ = m;
	int __i_3__ = i;
	int __j_4__ = j;

	{
		for (__m_2__ = 0; __m_2__ <= 4; __m_2__ += 1) {
			for (__i_3__ = 1; __i_3__ <= __arr_sub_0__ - 2; __i_3__ += 1) {
				for (__j_4__ = 1; __j_4__ <= __arr_sub_1__ - 2; __j_4__ += 1) {
					rhs[__m_2__][__i_3__][__j_4__][k] =
						rhs[__m_2__][__i_3__][__j_4__][k] -
						dssp * (5.0 * u[__m_2__][__i_3__][__j_4__][k] - 4.0 * u[__m_2__][__i_3__][__j_4__][k + 1] +
								u[__m_2__][__i_3__][__j_4__][k + 2]);
				}
			}
		}
	}

	m = __m_2__;
	i = __i_3__;
	j = __j_4__;
})

/**
 * @brief Run the benchmarks for sp_c_compute_rhs_line1630_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(sp_c_compute_rhs_line1630_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}