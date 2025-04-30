/**
 * @file codelets/codelet_polybench_bt_c_compute_rhs_line1881_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) bt_c_compute_rhs_line1881_0 codelet
 */

#include "bench_common.hpp"

int j = 63;
int grid_points[3] = {64, 64, 64};
int k = 63;
int m = 5;
double rhs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1][5];
int i = 2;
double dssp = 0.250000;
double u[(64 + 1) / 2 * 2 + 1][(64 + 1) / 2 * 2 + 1][(64 + 1) / 2 * 2 + 1][5];

OPTIMISE_SIZE
BENCHMARK_FN(bt_c_compute_rhs_line1881_loop_c_0_c, {
	int __arr_sub_0__ = grid_points[1];
	int __arr_sub_1__ = grid_points[2];
	int j2 = j;
	int k3 = k;
	int m4 = m;

	{
		for (j2 = 1; j2 <= __arr_sub_0__ - 1 - 1; j2 += 1) {
			for (k3 = 1; k3 <= __arr_sub_1__ - 1 - 1; k3 += 1) {
				for (m4 = 0; m4 <= 4; m4 += 1) {
					rhs[i][j2][k3][m4] = rhs[i][j2][k3][m4] - dssp * (-4.0 * u[i - 1][j2][k3][m4] + 6.0 * u[i][j2][k3][m4] -
																	  4.0 * u[i + 1][j2][k3][m4] + u[i + 2][j2][k3][m4]);
				}
			}
		}
	}

	j = j2;
	k = k3;
	m = m4;
})

/**
 * @brief Run the benchmarks for bt_c_compute_rhs_line1881_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(bt_c_compute_rhs_line1881_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}