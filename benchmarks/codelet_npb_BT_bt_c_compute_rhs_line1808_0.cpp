/**
 * @file benchmarks/codelet_polybench_bt_c_compute_rhs_line1808_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) bt_c_compute_rhs_line1808_0 codelet
 */

#include "bench_common.hpp"

int i = 64;
int grid_points[3] = {64, 64, 64};
int j = 64;
int k = 64;
double uijk = 0.000000;
double us[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double up1 = 0.000000;
double um1 = 0.000000;
double rhs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1][5];
double dx1tx1 = 2976.750000;
double u[(64 + 1) / 2 * 2 + 1][(64 + 1) / 2 * 2 + 1][(64 + 1) / 2 * 2 + 1][5];
double tx2 = 31.500000;
double dx2tx1 = 2976.750000;
double xxcon2 = 396.900000;
double con43 = 1.333333;
double square[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double c2 = 0.400000;
double dx3tx1 = 2976.750000;
double vs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double dx4tx1 = 2976.750000;
double ws[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double dx5tx1 = 2976.750000;
double xxcon3 = -381.024000;
double qs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double xxcon4 = 66.150000;
double xxcon5 = 777.924000;
double rho_i[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double c1 = 1.400000;

OPTIMISE_SIZE
BENCHMARK_FN(bt_c_compute_rhs_line1808_loop_c_0_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[1];
	int __arr_sub_2__ = grid_points[2];
	int i3 = i;
	int j4 = j;
	int k5 = k;

	{
		for (i3 = 1; i3 <= __arr_sub_0__ - 1 - 1; i3 += 1) {
			for (j4 = 1; j4 <= __arr_sub_1__ - 1 - 1; j4 += 1) {
				for (k5 = 1; k5 <= __arr_sub_2__ - 1 - 1; k5 += 1) {
					uijk = us[i3][j4][k5];
					up1 = us[i3 + 1][j4][k5];
					um1 = us[i3 - 1][j4][k5];
					rhs[i3][j4][k5][0] = rhs[i3][j4][k5][0] +
										 dx1tx1 * (u[i3 + 1][j4][k5][0] - 2.0 * u[i3][j4][k5][0] + u[i3 - 1][j4][k5][0]) -
										 tx2 * (u[i3 + 1][j4][k5][1] - u[i3 - 1][j4][k5][1]);
					rhs[i3][j4][k5][1] =
						rhs[i3][j4][k5][1] + dx2tx1 * (u[i3 + 1][j4][k5][1] - 2.0 * u[i3][j4][k5][1] + u[i3 - 1][j4][k5][1]) +
						xxcon2 * con43 * (up1 - 2.0 * uijk + um1) -
						tx2 * (u[i3 + 1][j4][k5][1] * up1 - u[i3 - 1][j4][k5][1] * um1 +
							   (u[i3 + 1][j4][k5][4] - square[i3 + 1][j4][k5] - u[i3 - 1][j4][k5][4] + square[i3 - 1][j4][k5]) * c2);
					rhs[i3][j4][k5][2] = rhs[i3][j4][k5][2] +
										 dx3tx1 * (u[i3 + 1][j4][k5][2] - 2.0 * u[i3][j4][k5][2] + u[i3 - 1][j4][k5][2]) +
										 xxcon2 * (vs[i3 + 1][j4][k5] - 2.0 * vs[i3][j4][k5] + vs[i3 - 1][j4][k5]) -
										 tx2 * (u[i3 + 1][j4][k5][2] * up1 - u[i3 - 1][j4][k5][2] * um1);
					rhs[i3][j4][k5][3] = rhs[i3][j4][k5][3] +
										 dx4tx1 * (u[i3 + 1][j4][k5][3] - 2.0 * u[i3][j4][k5][3] + u[i3 - 1][j4][k5][3]) +
										 xxcon2 * (ws[i3 + 1][j4][k5] - 2.0 * ws[i3][j4][k5] + ws[i3 - 1][j4][k5]) -
										 tx2 * (u[i3 + 1][j4][k5][3] * up1 - u[i3 - 1][j4][k5][3] * um1);
					rhs[i3][j4][k5][4] =
						rhs[i3][j4][k5][4] + dx5tx1 * (u[i3 + 1][j4][k5][4] - 2.0 * u[i3][j4][k5][4] + u[i3 - 1][j4][k5][4]) +
						xxcon3 * (qs[i3 + 1][j4][k5] - 2.0 * qs[i3][j4][k5] + qs[i3 - 1][j4][k5]) +
						xxcon4 * (up1 * up1 - 2.0 * uijk * uijk + um1 * um1) +
						xxcon5 * (u[i3 + 1][j4][k5][4] * rho_i[i3 + 1][j4][k5] - 2.0 * u[i3][j4][k5][4] * rho_i[i3][j4][k5] +
								  u[i3 - 1][j4][k5][4] * rho_i[i3 - 1][j4][k5]) -
						tx2 * ((c1 * u[i3 + 1][j4][k5][4] - c2 * square[i3 + 1][j4][k5]) * up1 -
							   (c1 * u[i3 - 1][j4][k5][4] - c2 * square[i3 - 1][j4][k5]) * um1);
				}
			}
		}
	}

	i = i3;
	j = j4;
	k = k5;
})

/**
 * @brief Run the benchmarks for bt_c_compute_rhs_line1808_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(bt_c_compute_rhs_line1808_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}