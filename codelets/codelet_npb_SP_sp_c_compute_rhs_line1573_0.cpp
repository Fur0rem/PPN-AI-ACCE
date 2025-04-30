/**
 * @file codelets/codelet_polybench_sp_c_compute_rhs_line1573_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) sp_c_compute_rhs_line1573_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
int grid_points[3] = {64, 64, 64};
int j = 0;
int k = 0;
double wijk = 0;
double ws[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double wp1 = 0;
double wm1 = 0;
double rhs[5][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double dz1tz1 = 3969.000000;
double u[5][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double tz2 = 31.500000;
double dz2tz1 = 3969.000000;
double zzcon2 = 396.900000;
double us[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double dz3tz1 = 3969.000000;
double vs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double dz4tz1 = 3969.000000;
double con43 = 1.333333;
double square[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double c2 = 0.400000;
double dz5tz1 = 3969.000000;
double zzcon3 = -381.024000;
double qs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double zzcon4 = 66.150000;
double zzcon5 = 777.924000;
double rho_i[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double c1 = 1.400000;

OPTIMISE_SIZE
BENCHMARK_FN(sp_c_compute_rhs_line1573_loop_c_0_c, {
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
					wijk = ws[i3][j4][k5];
					wp1 = ws[i3][j4][k5 + 1];
					wm1 = ws[i3][j4][k5 - 1];
					rhs[0][i3][j4][k5] = rhs[0][i3][j4][k5] +
										 dz1tz1 * (u[0][i3][j4][k5 + 1] - 2.0 * u[0][i3][j4][k5] + u[0][i3][j4][k5 - 1]) -
										 tz2 * (u[3][i3][j4][k5 + 1] - u[3][i3][j4][k5 - 1]);
					rhs[1][i3][j4][k5] = rhs[1][i3][j4][k5] +
										 dz2tz1 * (u[1][i3][j4][k5 + 1] - 2.0 * u[1][i3][j4][k5] + u[1][i3][j4][k5 - 1]) +
										 zzcon2 * (us[i3][j4][k5 + 1] - 2.0 * us[i3][j4][k5] + us[i3][j4][k5 - 1]) -
										 tz2 * (u[1][i3][j4][k5 + 1] * wp1 - u[1][i3][j4][k5 - 1] * wm1);
					rhs[2][i3][j4][k5] = rhs[2][i3][j4][k5] +
										 dz3tz1 * (u[2][i3][j4][k5 + 1] - 2.0 * u[2][i3][j4][k5] + u[2][i3][j4][k5 - 1]) +
										 zzcon2 * (vs[i3][j4][k5 + 1] - 2.0 * vs[i3][j4][k5] + vs[i3][j4][k5 - 1]) -
										 tz2 * (u[2][i3][j4][k5 + 1] * wp1 - u[2][i3][j4][k5 - 1] * wm1);
					rhs[3][i3][j4][k5] =
						rhs[3][i3][j4][k5] + dz4tz1 * (u[3][i3][j4][k5 + 1] - 2.0 * u[3][i3][j4][k5] + u[3][i3][j4][k5 - 1]) +
						zzcon2 * con43 * (wp1 - 2.0 * wijk + wm1) -
						tz2 * (u[3][i3][j4][k5 + 1] * wp1 - u[3][i3][j4][k5 - 1] * wm1 +
							   (u[4][i3][j4][k5 + 1] - square[i3][j4][k5 + 1] - u[4][i3][j4][k5 - 1] + square[i3][j4][k5 - 1]) * c2);
					rhs[4][i3][j4][k5] =
						rhs[4][i3][j4][k5] + dz5tz1 * (u[4][i3][j4][k5 + 1] - 2.0 * u[4][i3][j4][k5] + u[4][i3][j4][k5 - 1]) +
						zzcon3 * (qs[i3][j4][k5 + 1] - 2.0 * qs[i3][j4][k5] + qs[i3][j4][k5 - 1]) +
						zzcon4 * (wp1 * wp1 - 2.0 * wijk * wijk + wm1 * wm1) +
						zzcon5 * (u[4][i3][j4][k5 + 1] * rho_i[i3][j4][k5 + 1] - 2.0 * u[4][i3][j4][k5] * rho_i[i3][j4][k5] +
								  u[4][i3][j4][k5 - 1] * rho_i[i3][j4][k5 - 1]) -
						tz2 * ((c1 * u[4][i3][j4][k5 + 1] - c2 * square[i3][j4][k5 + 1]) * wp1 -
							   (c1 * u[4][i3][j4][k5 - 1] - c2 * square[i3][j4][k5 - 1]) * wm1);
				}
			}
		}
	}

	i = i3;
	j = j4;
	k = k5;
})

/**
 * @brief Run the benchmarks for sp_c_compute_rhs_line1573_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(sp_c_compute_rhs_line1573_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}