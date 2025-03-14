/**
 * @file benchmarks/codelet_polybench_bt_c_compute_rhs_line1771_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) bt_c_compute_rhs_line1771_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
int grid_points[3] = {64, 64, 64};
int j = 0;
int k = 0;
double rho_inv = 0.000000;
double u[(64 + 1) / 2 * 2 + 1][(64 + 1) / 2 * 2 + 1][(64 + 1) / 2 * 2 + 1][5];
double rho_i[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double us[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double vs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double ws[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double square[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double qs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];

BENCHMARK_FN(bt_c_compute_rhs_line1771_loop_c_0_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[1];
	int __arr_sub_2__ = grid_points[2];
	int __i_3__ = i;
	int __j_4__ = j;
	int __k_5__ = k;

	{
		for (__i_3__ = 0; __i_3__ <= __arr_sub_0__ - 1; __i_3__ += 1) {
			for (__j_4__ = 0; __j_4__ <= __arr_sub_1__ - 1; __j_4__ += 1) {
				for (__k_5__ = 0; __k_5__ <= __arr_sub_2__ - 1; __k_5__ += 1) {
					rho_inv = 1.0 / u[__i_3__][__j_4__][__k_5__][0];
					rho_i[__i_3__][__j_4__][__k_5__] = rho_inv;
					us[__i_3__][__j_4__][__k_5__] = u[__i_3__][__j_4__][__k_5__][1] * rho_inv;
					vs[__i_3__][__j_4__][__k_5__] = u[__i_3__][__j_4__][__k_5__][2] * rho_inv;
					ws[__i_3__][__j_4__][__k_5__] = u[__i_3__][__j_4__][__k_5__][3] * rho_inv;
					square[__i_3__][__j_4__][__k_5__] = 0.5 *
														(u[__i_3__][__j_4__][__k_5__][1] * u[__i_3__][__j_4__][__k_5__][1] +
														 u[__i_3__][__j_4__][__k_5__][2] * u[__i_3__][__j_4__][__k_5__][2] +
														 u[__i_3__][__j_4__][__k_5__][3] * u[__i_3__][__j_4__][__k_5__][3]) *
														rho_inv;
					qs[__i_3__][__j_4__][__k_5__] = square[__i_3__][__j_4__][__k_5__] * rho_inv;
				}
			}
		}
	}

	i = __i_3__;
	j = __j_4__;
	k = __k_5__;
})

/**
 * @brief Run the benchmarks for bt_c_compute_rhs_line1771_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(bt_c_compute_rhs_line1771_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}