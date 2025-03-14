/**
 * @file benchmarks/codelet_polybench_sp_c_compute_rhs_line1531_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) sp_c_compute_rhs_line1531_0 codelet
 */

#include "bench_common.hpp"

int m = 0;
int i = 0;
int grid_points[3] = {64, 64, 64};
int j = 0;
int k = 0;
double rhs[5][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double dssp = 0.250000;
double u[5][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];

OPTIMISE_SIZE
BENCHMARK_FN(sp_c_compute_rhs_line1531_loop_c_0_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[1];
	int __arr_sub_2__ = grid_points[2];
	int m3 = m;
	int i4 = i;
	int j5 = j;
	int k6 = k;

	for (m3 = 0; m3 <= 4; m3 += 1) {
		for (i4 = 1; i4 <= __arr_sub_0__ - 2; i4 += 1) {

			for (j5 = 3 * 1; j5 <= __arr_sub_1__ - 3 - 1; j5 += 1) {

				for (k6 = 1; k6 <= __arr_sub_2__ - 2; k6 += 1) {
					rhs[m3][i4][j5][k6] =
						rhs[m3][i4][j5][k6] - dssp * (u[m3][i4][j5 - 2][k6] - 4.0 * u[m3][i4][j5 - 1][k6] + 6.0 * u[m3][i4][j5][k6] -
													  4.0 * u[m3][i4][j5 + 1][k6] + u[m3][i4][j5 + 2][k6]);
				}
			}
		}
	}

	m = m3;
	i = i4;
	j = j5;
	k = k6;
})

/**
 * @brief Run the benchmarks for sp_c_compute_rhs_line1531_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(sp_c_compute_rhs_line1531_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}