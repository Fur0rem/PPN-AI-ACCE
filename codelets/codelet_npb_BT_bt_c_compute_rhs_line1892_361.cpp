/**
 * @file codelets/codelet_polybench_bt_c_compute_rhs_line1892_361.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) bt_c_compute_rhs_line1892_361 codelet
 */

#include "bench_common.hpp"

int i = 2;
int grid_points[3] = {64, 64, 64};
int j = 63;
int k = 63;
int m = 5;
double rhs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1][5];
double dssp = 0.250000;
double u[(64 + 1) / 2 * 2 + 1][(64 + 1) / 2 * 2 + 1][(64 + 1) / 2 * 2 + 1][5];

OPTIMISE_SIZE
BENCHMARK_FN(bt_c_compute_rhs_line1892_loop_c_361_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[1];
	int __arr_sub_2__ = grid_points[2];
	int i3 = i;
	int j4 = j;
	int k5 = k;
	int m6 = m;

	{
		for (j4 = 1; j4 <= __arr_sub_1__ - 1 - 1; j4 += 1) {
			/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
			/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
			int _lu_fringe_537 = (((__arr_sub_0__ - 3 - 1 + 1 - 3) % 1 == 0 ? (__arr_sub_0__ - 3 - 1 + 1 - 3) / 1
																			: (__arr_sub_0__ - 3 - 1 + 1 - 3) / 1 + 1)) %
											 2 ==
										 0
									 ? 0
									 : 2;
			for (i3 = 3; i3 <= __arr_sub_0__ - 3 - 1 - _lu_fringe_537; i3 += 2) {
				for (k5 = 1; k5 <= __arr_sub_2__ - 1 - 1; k5 += 1) {
					for (m6 = 0; m6 <= 4; m6 += 1) {
						rhs[i3][j4][k5][m6] =
							rhs[i3][j4][k5][m6] - dssp * (u[i3 - 2][j4][k5][m6] - 4.0 * u[i3 - 1][j4][k5][m6] + 6.0 * u[i3][j4][k5][m6] -
														  4.0 * u[i3 + 1][j4][k5][m6] + u[i3 + 2][j4][k5][m6]);
					}
					for (m6 = 0; m6 <= 4; m6 += 1) {
						rhs[i3 + 1][j4][k5][m6] =
							rhs[i3 + 1][j4][k5][m6] -
							dssp * (u[i3 + 1 - 2][j4][k5][m6] - 4.0 * u[i3 + 1 - 1][j4][k5][m6] + 6.0 * u[i3 + 1][j4][k5][m6] -
									4.0 * u[i3 + 1 + 1][j4][k5][m6] + u[i3 + 1 + 2][j4][k5][m6]);
					}
				}
			}
			for (; i3 <= __arr_sub_0__ - 3 - 1; i3 += 1) {
				for (k5 = 1; k5 <= __arr_sub_2__ - 1 - 1; k5 += 1) {
					for (m6 = 0; m6 <= 4; m6 += 1) {
						rhs[i3][j4][k5][m6] =
							rhs[i3][j4][k5][m6] - dssp * (u[i3 - 2][j4][k5][m6] - 4.0 * u[i3 - 1][j4][k5][m6] + 6.0 * u[i3][j4][k5][m6] -
														  4.0 * u[i3 + 1][j4][k5][m6] + u[i3 + 2][j4][k5][m6]);
					}
				}
			}
		}
	}

	i = i3;
	j = j4;
	k = k5;
	m = m6;
})

/**
 * @brief Run the benchmarks for bt_c_compute_rhs_line1892_loop_c_361_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(bt_c_compute_rhs_line1892_loop_c_361_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}