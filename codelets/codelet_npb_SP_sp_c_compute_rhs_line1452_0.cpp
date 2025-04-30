/**
 * @file codelets/codelet_polybench_sp_c_compute_rhs_line1452_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) sp_c_compute_rhs_line1452_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
int grid_points[3] = {64, 64, 64};
int j = 0;
int k = 0;
double vijk = 0;
double vs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double vp1 = 0;
double vm1 = 0;
double rhs[5][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double dy1ty1 = 2976.750000;
double u[5][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double ty2 = 31.500000;
double dy2ty1 = 2976.750000;
double yycon2 = 396.900000;
double us[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double dy3ty1 = 2976.750000;
double con43 = 1.333333;
double square[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double c2 = 0.400000;
double dy4ty1 = 2976.750000;
double ws[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double dy5ty1 = 2976.750000;
double yycon3 = -381.024000;
double qs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double yycon4 = 66.150000;
double yycon5 = 777.924000;
double rho_i[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double c1 = 1.400000;

OPTIMISE_SIZE
BENCHMARK_FN(sp_c_compute_rhs_line1452_loop_c_0_c, {
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
					vijk = vs[i3][j4][k5];
					vp1 = vs[i3][j4 + 1][k5];
					vm1 = vs[i3][j4 - 1][k5];
					rhs[0][i3][j4][k5] = rhs[0][i3][j4][k5] +
										 dy1ty1 * (u[0][i3][j4 + 1][k5] - 2.0 * u[0][i3][j4][k5] + u[0][i3][j4 - 1][k5]) -
										 ty2 * (u[2][i3][j4 + 1][k5] - u[2][i3][j4 - 1][k5]);
					rhs[1][i3][j4][k5] = rhs[1][i3][j4][k5] +
										 dy2ty1 * (u[1][i3][j4 + 1][k5] - 2.0 * u[1][i3][j4][k5] + u[1][i3][j4 - 1][k5]) +
										 yycon2 * (us[i3][j4 + 1][k5] - 2.0 * us[i3][j4][k5] + us[i3][j4 - 1][k5]) -
										 ty2 * (u[1][i3][j4 + 1][k5] * vp1 - u[1][i3][j4 - 1][k5] * vm1);
					rhs[2][i3][j4][k5] =
						rhs[2][i3][j4][k5] + dy3ty1 * (u[2][i3][j4 + 1][k5] - 2.0 * u[2][i3][j4][k5] + u[2][i3][j4 - 1][k5]) +
						yycon2 * con43 * (vp1 - 2.0 * vijk + vm1) -
						ty2 * (u[2][i3][j4 + 1][k5] * vp1 - u[2][i3][j4 - 1][k5] * vm1 +
							   (u[4][i3][j4 + 1][k5] - square[i3][j4 + 1][k5] - u[4][i3][j4 - 1][k5] + square[i3][j4 - 1][k5]) * c2);
					rhs[3][i3][j4][k5] = rhs[3][i3][j4][k5] +
										 dy4ty1 * (u[3][i3][j4 + 1][k5] - 2.0 * u[3][i3][j4][k5] + u[3][i3][j4 - 1][k5]) +
										 yycon2 * (ws[i3][j4 + 1][k5] - 2.0 * ws[i3][j4][k5] + ws[i3][j4 - 1][k5]) -
										 ty2 * (u[3][i3][j4 + 1][k5] * vp1 - u[3][i3][j4 - 1][k5] * vm1);
					rhs[4][i3][j4][k5] =
						rhs[4][i3][j4][k5] + dy5ty1 * (u[4][i3][j4 + 1][k5] - 2.0 * u[4][i3][j4][k5] + u[4][i3][j4 - 1][k5]) +
						yycon3 * (qs[i3][j4 + 1][k5] - 2.0 * qs[i3][j4][k5] + qs[i3][j4 - 1][k5]) +
						yycon4 * (vp1 * vp1 - 2.0 * vijk * vijk + vm1 * vm1) +
						yycon5 * (u[4][i3][j4 + 1][k5] * rho_i[i3][j4 + 1][k5] - 2.0 * u[4][i3][j4][k5] * rho_i[i3][j4][k5] +
								  u[4][i3][j4 - 1][k5] * rho_i[i3][j4 - 1][k5]) -
						ty2 * ((c1 * u[4][i3][j4 + 1][k5] - c2 * square[i3][j4 + 1][k5]) * vp1 -
							   (c1 * u[4][i3][j4 - 1][k5] - c2 * square[i3][j4 - 1][k5]) * vm1);
				}
			}
		}
	}

	i = i3;
	j = j4;
	k = k5;
})

/**
 * @brief Run the benchmarks for sp_c_compute_rhs_line1452_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(sp_c_compute_rhs_line1452_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}