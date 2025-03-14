/**
 * @file benchmarks/codelet_polybench_bt_c_compute_rhs_line1933_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) bt_c_compute_rhs_line1933_0 codelet
 */

#include "bench_common.hpp"

int i = 62;
int grid_points[3] = {64, 64, 64};
int j = 63;
int k = 63;
double vijk = 0.000000;
double vs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double vp1 = 0.000000;
double vm1 = 0.000000;
double rhs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1][5];
double dy1ty1 = 2976.750000;
double u[(64 + 1) / 2 * 2 + 1][(64 + 1) / 2 * 2 + 1][(64 + 1) / 2 * 2 + 1][5];
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
BENCHMARK_FN(bt_c_compute_rhs_line1933_loop_c_0_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[1];
	int __arr_sub_2__ = grid_points[2];
	int __i_3__ = i;
	int __j_4__ = j;
	int __k_5__ = k;

	{
		for (__i_3__ = 1; __i_3__ <= __arr_sub_0__ - 1 - 1; __i_3__ += 1) {
			for (__j_4__ = 1; __j_4__ <= __arr_sub_1__ - 1 - 1; __j_4__ += 1) {
				for (__k_5__ = 1; __k_5__ <= __arr_sub_2__ - 1 - 1; __k_5__ += 1) {
					vijk = vs[__i_3__][__j_4__][__k_5__];
					vp1 = vs[__i_3__][__j_4__ + 1][__k_5__];
					vm1 = vs[__i_3__][__j_4__ - 1][__k_5__];
					rhs[__i_3__][__j_4__][__k_5__][0] =
						rhs[__i_3__][__j_4__][__k_5__][0] +
						dy1ty1 * (u[__i_3__][__j_4__ + 1][__k_5__][0] - 2.0 * u[__i_3__][__j_4__][__k_5__][0] +
								  u[__i_3__][__j_4__ - 1][__k_5__][0]) -
						ty2 * (u[__i_3__][__j_4__ + 1][__k_5__][2] - u[__i_3__][__j_4__ - 1][__k_5__][2]);
					rhs[__i_3__][__j_4__][__k_5__][1] =
						rhs[__i_3__][__j_4__][__k_5__][1] +
						dy2ty1 * (u[__i_3__][__j_4__ + 1][__k_5__][1] - 2.0 * u[__i_3__][__j_4__][__k_5__][1] +
								  u[__i_3__][__j_4__ - 1][__k_5__][1]) +
						yycon2 *
							(us[__i_3__][__j_4__ + 1][__k_5__] - 2.0 * us[__i_3__][__j_4__][__k_5__] + us[__i_3__][__j_4__ - 1][__k_5__]) -
						ty2 * (u[__i_3__][__j_4__ + 1][__k_5__][1] * vp1 - u[__i_3__][__j_4__ - 1][__k_5__][1] * vm1);
					rhs[__i_3__][__j_4__][__k_5__][2] =
						rhs[__i_3__][__j_4__][__k_5__][2] +
						dy3ty1 * (u[__i_3__][__j_4__ + 1][__k_5__][2] - 2.0 * u[__i_3__][__j_4__][__k_5__][2] +
								  u[__i_3__][__j_4__ - 1][__k_5__][2]) +
						yycon2 * con43 * (vp1 - 2.0 * vijk + vm1) -
						ty2 * (u[__i_3__][__j_4__ + 1][__k_5__][2] * vp1 - u[__i_3__][__j_4__ - 1][__k_5__][2] * vm1 +
							   (u[__i_3__][__j_4__ + 1][__k_5__][4] - square[__i_3__][__j_4__ + 1][__k_5__] -
								u[__i_3__][__j_4__ - 1][__k_5__][4] + square[__i_3__][__j_4__ - 1][__k_5__]) *
								   c2);
					rhs[__i_3__][__j_4__][__k_5__][3] =
						rhs[__i_3__][__j_4__][__k_5__][3] +
						dy4ty1 * (u[__i_3__][__j_4__ + 1][__k_5__][3] - 2.0 * u[__i_3__][__j_4__][__k_5__][3] +
								  u[__i_3__][__j_4__ - 1][__k_5__][3]) +
						yycon2 *
							(ws[__i_3__][__j_4__ + 1][__k_5__] - 2.0 * ws[__i_3__][__j_4__][__k_5__] + ws[__i_3__][__j_4__ - 1][__k_5__]) -
						ty2 * (u[__i_3__][__j_4__ + 1][__k_5__][3] * vp1 - u[__i_3__][__j_4__ - 1][__k_5__][3] * vm1);
					rhs[__i_3__][__j_4__][__k_5__][4] =
						rhs[__i_3__][__j_4__][__k_5__][4] +
						dy5ty1 * (u[__i_3__][__j_4__ + 1][__k_5__][4] - 2.0 * u[__i_3__][__j_4__][__k_5__][4] +
								  u[__i_3__][__j_4__ - 1][__k_5__][4]) +
						yycon3 *
							(qs[__i_3__][__j_4__ + 1][__k_5__] - 2.0 * qs[__i_3__][__j_4__][__k_5__] + qs[__i_3__][__j_4__ - 1][__k_5__]) +
						yycon4 * (vp1 * vp1 - 2.0 * vijk * vijk + vm1 * vm1) +
						yycon5 * (u[__i_3__][__j_4__ + 1][__k_5__][4] * rho_i[__i_3__][__j_4__ + 1][__k_5__] -
								  2.0 * u[__i_3__][__j_4__][__k_5__][4] * rho_i[__i_3__][__j_4__][__k_5__] +
								  u[__i_3__][__j_4__ - 1][__k_5__][4] * rho_i[__i_3__][__j_4__ - 1][__k_5__]) -
						ty2 * ((c1 * u[__i_3__][__j_4__ + 1][__k_5__][4] - c2 * square[__i_3__][__j_4__ + 1][__k_5__]) * vp1 -
							   (c1 * u[__i_3__][__j_4__ - 1][__k_5__][4] - c2 * square[__i_3__][__j_4__ - 1][__k_5__]) * vm1);
				}
			}
		}
	}

	i = __i_3__;
	j = __j_4__;
	k = __k_5__;
})

/**
 * @brief Run the benchmarks for bt_c_compute_rhs_line1933_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(bt_c_compute_rhs_line1933_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}