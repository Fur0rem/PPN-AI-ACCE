/**
 * @file benchmarks/codelet_polybench_bt_c_compute_rhs_line2053_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) bt_c_compute_rhs_line2053_0 codelet
 */

#include "bench_common.hpp"

int i = 63;
int grid_points[3] = {64, 64, 64};
int j = 62;
int k = 63;
double wijk = 0.000000;
double ws[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double wp1 = 0.000000;
double wm1 = 0.000000;
double rhs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1][5];
double dz1tz1 = 3969.000000;
double u[(64 + 1) / 2 * 2 + 1][(64 + 1) / 2 * 2 + 1][(64 + 1) / 2 * 2 + 1][5];
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
BENCHMARK_FN(bt_c_compute_rhs_line2053_loop_c_0_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[1];
	int __arr_sub_2__ = grid_points[2];
	int __i_3__ = i;
	int __j_4__ = j;
	int __k_5__ = k;

	{
		NO_VECTORISE_NO_UNROLL
		for (__i_3__ = 1; __i_3__ <= __arr_sub_0__ - 1 - 1; __i_3__ += 1) {
			NO_VECTORISE_NO_UNROLL
			for (__j_4__ = 1; __j_4__ <= __arr_sub_1__ - 1 - 1; __j_4__ += 1) {
				NO_VECTORISE_NO_UNROLL
				for (__k_5__ = 1; __k_5__ <= __arr_sub_2__ - 1 - 1; __k_5__ += 1) {
					wijk = ws[__i_3__][__j_4__][__k_5__];
					wp1 = ws[__i_3__][__j_4__][__k_5__ + 1];
					wm1 = ws[__i_3__][__j_4__][__k_5__ - 1];
					rhs[__i_3__][__j_4__][__k_5__][0] =
						rhs[__i_3__][__j_4__][__k_5__][0] +
						dz1tz1 * (u[__i_3__][__j_4__][__k_5__ + 1][0] - 2.0 * u[__i_3__][__j_4__][__k_5__][0] +
								  u[__i_3__][__j_4__][__k_5__ - 1][0]) -
						tz2 * (u[__i_3__][__j_4__][__k_5__ + 1][3] - u[__i_3__][__j_4__][__k_5__ - 1][3]);
					rhs[__i_3__][__j_4__][__k_5__][1] =
						rhs[__i_3__][__j_4__][__k_5__][1] +
						dz2tz1 * (u[__i_3__][__j_4__][__k_5__ + 1][1] - 2.0 * u[__i_3__][__j_4__][__k_5__][1] +
								  u[__i_3__][__j_4__][__k_5__ - 1][1]) +
						zzcon2 *
							(us[__i_3__][__j_4__][__k_5__ + 1] - 2.0 * us[__i_3__][__j_4__][__k_5__] + us[__i_3__][__j_4__][__k_5__ - 1]) -
						tz2 * (u[__i_3__][__j_4__][__k_5__ + 1][1] * wp1 - u[__i_3__][__j_4__][__k_5__ - 1][1] * wm1);
					rhs[__i_3__][__j_4__][__k_5__][2] =
						rhs[__i_3__][__j_4__][__k_5__][2] +
						dz3tz1 * (u[__i_3__][__j_4__][__k_5__ + 1][2] - 2.0 * u[__i_3__][__j_4__][__k_5__][2] +
								  u[__i_3__][__j_4__][__k_5__ - 1][2]) +
						zzcon2 *
							(vs[__i_3__][__j_4__][__k_5__ + 1] - 2.0 * vs[__i_3__][__j_4__][__k_5__] + vs[__i_3__][__j_4__][__k_5__ - 1]) -
						tz2 * (u[__i_3__][__j_4__][__k_5__ + 1][2] * wp1 - u[__i_3__][__j_4__][__k_5__ - 1][2] * wm1);
					rhs[__i_3__][__j_4__][__k_5__][3] =
						rhs[__i_3__][__j_4__][__k_5__][3] +
						dz4tz1 * (u[__i_3__][__j_4__][__k_5__ + 1][3] - 2.0 * u[__i_3__][__j_4__][__k_5__][3] +
								  u[__i_3__][__j_4__][__k_5__ - 1][3]) +
						zzcon2 * con43 * (wp1 - 2.0 * wijk + wm1) -
						tz2 * (u[__i_3__][__j_4__][__k_5__ + 1][3] * wp1 - u[__i_3__][__j_4__][__k_5__ - 1][3] * wm1 +
							   (u[__i_3__][__j_4__][__k_5__ + 1][4] - square[__i_3__][__j_4__][__k_5__ + 1] -
								u[__i_3__][__j_4__][__k_5__ - 1][4] + square[__i_3__][__j_4__][__k_5__ - 1]) *
								   c2);
					rhs[__i_3__][__j_4__][__k_5__][4] =
						rhs[__i_3__][__j_4__][__k_5__][4] +
						dz5tz1 * (u[__i_3__][__j_4__][__k_5__ + 1][4] - 2.0 * u[__i_3__][__j_4__][__k_5__][4] +
								  u[__i_3__][__j_4__][__k_5__ - 1][4]) +
						zzcon3 *
							(qs[__i_3__][__j_4__][__k_5__ + 1] - 2.0 * qs[__i_3__][__j_4__][__k_5__] + qs[__i_3__][__j_4__][__k_5__ - 1]) +
						zzcon4 * (wp1 * wp1 - 2.0 * wijk * wijk + wm1 * wm1) +
						zzcon5 * (u[__i_3__][__j_4__][__k_5__ + 1][4] * rho_i[__i_3__][__j_4__][__k_5__ + 1] -
								  2.0 * u[__i_3__][__j_4__][__k_5__][4] * rho_i[__i_3__][__j_4__][__k_5__] +
								  u[__i_3__][__j_4__][__k_5__ - 1][4] * rho_i[__i_3__][__j_4__][__k_5__ - 1]) -
						tz2 * ((c1 * u[__i_3__][__j_4__][__k_5__ + 1][4] - c2 * square[__i_3__][__j_4__][__k_5__ + 1]) * wp1 -
							   (c1 * u[__i_3__][__j_4__][__k_5__ - 1][4] - c2 * square[__i_3__][__j_4__][__k_5__ - 1]) * wm1);
				}
			}
		}
	}

	i = __i_3__;
	j = __j_4__;
	k = __k_5__;
})

/**
 * @brief Run the benchmarks for bt_c_compute_rhs_line2053_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(bt_c_compute_rhs_line2053_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}