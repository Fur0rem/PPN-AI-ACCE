/**
 * @file benchmarks/codelet_polybench_sp_c_compute_rhs_line1327_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) sp_c_compute_rhs_line1327_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
int grid_points[3] = {64, 64, 64};
int j = 0;
int k = 0;
double uijk = 0;
double us[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double up1 = 0;
double um1 = 0;
double rhs[5][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double dx1tx1 = 2976.750000;
double u[5][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
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
BENCHMARK_FN(sp_c_compute_rhs_line1327_loop_c_0_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[1];
	int __arr_sub_2__ = grid_points[2];
	int i3 = i;
	int j4 = j;
	int k5 = k;

	{
		for (i3 = 1; i3 <= __arr_sub_0__ - 2; i3 += 1) {

			for (j4 = 1; j4 <= __arr_sub_1__ - 2; j4 += 1) {

				for (k5 = 1; k5 <= __arr_sub_2__ - 2; k5 += 1) {
					uijk = us[i3][j4][k5];
					up1 = us[i3 + 1][j4][k5];
					um1 = us[i3 - 1][j4][k5];
					rhs[0][i3][j4][k5] = rhs[0][i3][j4][k5] +
										 dx1tx1 * (u[0][i3 + 1][j4][k5] - 2.0 * u[0][i3][j4][k5] + u[0][i3 - 1][j4][k5]) -
										 tx2 * (u[1][i3 + 1][j4][k5] - u[1][i3 - 1][j4][k5]);
					rhs[1][i3][j4][k5] =
						rhs[1][i3][j4][k5] + dx2tx1 * (u[1][i3 + 1][j4][k5] - 2.0 * u[1][i3][j4][k5] + u[1][i3 - 1][j4][k5]) +
						xxcon2 * con43 * (up1 - 2.0 * uijk + um1) -
						tx2 * (u[1][i3 + 1][j4][k5] * up1 - u[1][i3 - 1][j4][k5] * um1 +
							   (u[4][i3 + 1][j4][k5] - square[i3 + 1][j4][k5] - u[4][i3 - 1][j4][k5] + square[i3 - 1][j4][k5]) * c2);
					rhs[2][i3][j4][k5] = rhs[2][i3][j4][k5] +
										 dx3tx1 * (u[2][i3 + 1][j4][k5] - 2.0 * u[2][i3][j4][k5] + u[2][i3 - 1][j4][k5]) +
										 xxcon2 * (vs[i3 + 1][j4][k5] - 2.0 * vs[i3][j4][k5] + vs[i3 - 1][j4][k5]) -
										 tx2 * (u[2][i3 + 1][j4][k5] * up1 - u[2][i3 - 1][j4][k5] * um1);
					rhs[3][i3][j4][k5] = rhs[3][i3][j4][k5] +
										 dx4tx1 * (u[3][i3 + 1][j4][k5] - 2.0 * u[3][i3][j4][k5] + u[3][i3 - 1][j4][k5]) +
										 xxcon2 * (ws[i3 + 1][j4][k5] - 2.0 * ws[i3][j4][k5] + ws[i3 - 1][j4][k5]) -
										 tx2 * (u[3][i3 + 1][j4][k5] * up1 - u[3][i3 - 1][j4][k5] * um1);
					rhs[4][i3][j4][k5] =
						rhs[4][i3][j4][k5] + dx5tx1 * (u[4][i3 + 1][j4][k5] - 2.0 * u[4][i3][j4][k5] + u[4][i3 - 1][j4][k5]) +
						xxcon3 * (qs[i3 + 1][j4][k5] - 2.0 * qs[i3][j4][k5] + qs[i3 - 1][j4][k5]) +
						xxcon4 * (up1 * up1 - 2.0 * uijk * uijk + um1 * um1) +
						xxcon5 * (u[4][i3 + 1][j4][k5] * rho_i[i3 + 1][j4][k5] - 2.0 * u[4][i3][j4][k5] * rho_i[i3][j4][k5] +
								  u[4][i3 - 1][j4][k5] * rho_i[i3 - 1][j4][k5]) -
						tx2 * ((c1 * u[4][i3 + 1][j4][k5] - c2 * square[i3 + 1][j4][k5]) * up1 -
							   (c1 * u[4][i3 - 1][j4][k5] - c2 * square[i3 - 1][j4][k5]) * um1);
				}
			}
		}
	}

	i = i3;
	j = j4;
	k = k5;
})

/**
 * @brief Run the benchmarks for sp_c_compute_rhs_line1327_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(sp_c_compute_rhs_line1327_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}