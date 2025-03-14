/**
 * @file benchmarks/codelet_polybench_bt_c_compute_rhs_line1869_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) bt_c_compute_rhs_line1869_0 codelet
 */

#include "bench_common.hpp"

int j = 63;
int grid_points[3] = {64, 64, 64};
int k = 63;
int m = 5;
double rhs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1][5];
int i = 1;
double dssp = 0.250000;
double u[(64 + 1) / 2 * 2 + 1][(64 + 1) / 2 * 2 + 1][(64 + 1) / 2 * 2 + 1][5];

BENCHMARK_FN(bt_c_compute_rhs_line1869_loop_c_0_c, {
	int __arr_sub_0__ = grid_points[1];
	int __arr_sub_1__ = grid_points[2];
	int __j_2__ = j;
	int __k_3__ = k;
	int __m_4__ = m;

	{
		for (__j_2__ = 1; __j_2__ <= __arr_sub_0__ - 1 - 1; __j_2__ += 1) {
			for (__k_3__ = 1; __k_3__ <= __arr_sub_1__ - 1 - 1; __k_3__ += 1) {
				for (__m_4__ = 0; __m_4__ <= 4; __m_4__ += 1) {
					rhs[i][__j_2__][__k_3__][__m_4__] =
						rhs[i][__j_2__][__k_3__][__m_4__] -
						dssp * (5.0 * u[i][__j_2__][__k_3__][__m_4__] - 4.0 * u[i + 1][__j_2__][__k_3__][__m_4__] +
								u[i + 2][__j_2__][__k_3__][__m_4__]);
				}
			}
		}
	}

	j = __j_2__;
	k = __k_3__;
	m = __m_4__;
})

/**
 * @brief Run the benchmarks for bt_c_compute_rhs_line1869_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(bt_c_compute_rhs_line1869_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}