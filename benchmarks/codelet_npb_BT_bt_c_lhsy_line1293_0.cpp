/**
 * @file benchmarks/codelet_polybench_bt_c_lhsy_line1293_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) bt_c_lhsy_line1293_0 codelet
 */

#include "bench_common.hpp"

int i = 63;
int grid_points[3] = {64, 64, 64};
int j = 64;
int k = 63;
double dt = 0.000800;
double ty1 = 3969.000000;
double ty2 = 31.500000;
double lhs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1][3][5][5];
double fjac[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 - 1 + 1][5][5];
double njac[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 - 1 + 1][5][5];
double dy1 = 0.750000;
double dy2 = 0.750000;
double dy3 = 0.750000;
double dy4 = 0.750000;
double dy5 = 0.750000;

OPTIMISE_SIZE
BENCHMARK_FN(bt_c_lhsy_line1293_loop_c_0_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[1];
	int __arr_sub_2__ = grid_points[2];
	int i_3 = i;
	double tmp1;
	double tmp2;
	int j_6 = j;
	int k_7 = k;

	{
		for (i_3 = 1; i_3 <= __arr_sub_0__ - 1 - 1; i_3 += 1) {

			for (j_6 = 1; j_6 <= __arr_sub_1__ - 1 - 1; j_6 += 1) {

				for (k_7 = 1; k_7 <= __arr_sub_2__ - 1 - 1; k_7 += 1) {
					tmp1 = dt * ty1;
					tmp2 = dt * ty2;
					// lhs[i_3][j_6][k_7][0][0][0] = -tmp2 * fjac[i_3][j_6 - 1][k_7][0][0] - tmp1 * njac[i_3][j_6 - 1][k_7][0][0] - tmp1 *
					// dy1; lhs[i_3][j_6][k_7][0][0][1] = -tmp2 * fjac[i_3][j_6 - 1][k_7][0][1] - tmp1 * njac[i_3][j_6 - 1][k_7][0][1];
					// lhs[i_3][j_6][k_7][0][0][2] = -tmp2 * fjac[i_3][j_6 - 1][k_7][0][2] - tmp1 * njac[i_3][j_6 - 1][k_7][0][2];
					// lhs[i_3][j_6][k_7][0][0][3] = -tmp2 * fjac[i_3][j_6 - 1][k_7][0][3] - tmp1 * njac[i_3][j_6 - 1][k_7][0][3];
					// lhs[i_3][j_6][k_7][0][0][4] = -tmp2 * fjac[i_3][j_6 - 1][k_7][0][4] - tmp1 * njac[i_3][j_6 - 1][k_7][0][4];
					// lhs[i_3][j_6][k_7][0][1][0] = -tmp2 * fjac[i_3][j_6 - 1][k_7][1][0] - tmp1 * njac[i_3][j_6 - 1][k_7][1][0];
					// lhs[i_3][j_6][k_7][0][1][1] = -tmp2 * fjac[i_3][j_6 - 1][k_7][1][1] - tmp1 * njac[i_3][j_6 - 1][k_7][1][1] - tmp1 *
					// dy2; lhs[i_3][j_6][k_7][0][1][2] = -tmp2 * fjac[i_3][j_6 - 1][k_7][1][2] - tmp1 * njac[i_3][j_6 - 1][k_7][1][2];
					// lhs[i_3][j_6][k_7][0][1][3] = -tmp2 * fjac[i_3][j_6 - 1][k_7][1][3] - tmp1 * njac[i_3][j_6 - 1][k_7][1][3];
					// lhs[i_3][j_6][k_7][0][1][4] = -tmp2 * fjac[i_3][j_6 - 1][k_7][1][4] - tmp1 * njac[i_3][j_6 - 1][k_7][1][4];
					// lhs[i_3][j_6][k_7][0][2][0] = -tmp2 * fjac[i_3][j_6 - 1][k_7][2][0] - tmp1 * njac[i_3][j_6 - 1][k_7][2][0];
					// lhs[i_3][j_6][k_7][0][2][1] = -tmp2 * fjac[i_3][j_6 - 1][k_7][2][1] - tmp1 * njac[i_3][j_6 - 1][k_7][2][1];
					// lhs[i_3][j_6][k_7][0][2][2] = -tmp2 * fjac[i_3][j_6 - 1][k_7][2][2] - tmp1 * njac[i_3][j_6 - 1][k_7][2][2] - tmp1 *
					// dy3; lhs[i_3][j_6][k_7][0][2][3] = -tmp2 * fjac[i_3][j_6 - 1][k_7][2][3] - tmp1 * njac[i_3][j_6 - 1][k_7][2][3];
					// lhs[i_3][j_6][k_7][0][2][4] = -tmp2 * fjac[i_3][j_6 - 1][k_7][2][4] - tmp1 * njac[i_3][j_6 - 1][k_7][2][4];
					// lhs[i_3][j_6][k_7][0][3][0] = -tmp2 * fjac[i_3][j_6 - 1][k_7][3][0] - tmp1 * njac[i_3][j_6 - 1][k_7][3][0];
					// lhs[i_3][j_6][k_7][0][3][1] = -tmp2 * fjac[i_3][j_6 - 1][k_7][3][1] - tmp1 * njac[i_3][j_6 - 1][k_7][3][1];
					// lhs[i_3][j_6][k_7][0][3][2] = -tmp2 * fjac[i_3][j_6 - 1][k_7][3][2] - tmp1 * njac[i_3][j_6 - 1][k_7][3][2];
					// lhs[i_3][j_6][k_7][0][3][3] = -tmp2 * fjac[i_3][j_6 - 1][k_7][3][3] - tmp1 * njac[i_3][j_6 - 1][k_7][3][3] - tmp1 *
					// dy4; lhs[i_3][j_6][k_7][0][3][4] = -tmp2 * fjac[i_3][j_6 - 1][k_7][3][4] - tmp1 * njac[i_3][j_6 - 1][k_7][3][4];
					// lhs[i_3][j_6][k_7][0][4][0] = -tmp2 * fjac[i_3][j_6 - 1][k_7][4][0] - tmp1 * njac[i_3][j_6 - 1][k_7][4][0];
					// lhs[i_3][j_6][k_7][0][4][1] = -tmp2 * fjac[i_3][j_6 - 1][k_7][4][1] - tmp1 * njac[i_3][j_6 - 1][k_7][4][1];
					// lhs[i_3][j_6][k_7][0][4][2] = -tmp2 * fjac[i_3][j_6 - 1][k_7][4][2] - tmp1 * njac[i_3][j_6 - 1][k_7][4][2];
					// lhs[i_3][j_6][k_7][0][4][3] = -tmp2 * fjac[i_3][j_6 - 1][k_7][4][3] - tmp1 * njac[i_3][j_6 - 1][k_7][4][3];
					// lhs[i_3][j_6][k_7][0][4][4] = -tmp2 * fjac[i_3][j_6 - 1][k_7][4][4] - tmp1 * njac[i_3][j_6 - 1][k_7][4][4] - tmp1 *
					// dy5;

					for (int x1 = 0; x1 <= 4; x1 += 1) {
						for (int x2 = 0; x2 <= 4; x2 += 1) {
							lhs[i_3][j_6][k_7][0][x1][x2] =
								-tmp2 * fjac[i_3][j_6 - 1][k_7][x1][x2] - tmp1 * njac[i_3][j_6 - 1][k_7][x1][x2];
						}
					}
					lhs[i_3][j_6][k_7][1][0][0] -= tmp1 * dy1;
					lhs[i_3][j_6][k_7][1][1][1] -= tmp1 * dy2;
					lhs[i_3][j_6][k_7][1][2][2] -= tmp1 * dy3;
					lhs[i_3][j_6][k_7][1][3][3] -= tmp1 * dy4;
					lhs[i_3][j_6][k_7][1][4][4] -= tmp1 * dy5;

					// lhs[i_3][j_6][k_7][1][0][0] = 1.0 + tmp1 * 2.0 * njac[i_3][j_6][k_7][0][0] + tmp1 * 2.0 * dy1;
					// lhs[i_3][j_6][k_7][1][0][1] = tmp1 * 2.0 * njac[i_3][j_6][k_7][0][1];
					// lhs[i_3][j_6][k_7][1][0][2] = tmp1 * 2.0 * njac[i_3][j_6][k_7][0][2];
					// lhs[i_3][j_6][k_7][1][0][3] = tmp1 * 2.0 * njac[i_3][j_6][k_7][0][3];
					// lhs[i_3][j_6][k_7][1][0][4] = tmp1 * 2.0 * njac[i_3][j_6][k_7][0][4];
					// lhs[i_3][j_6][k_7][1][1][0] = tmp1 * 2.0 * njac[i_3][j_6][k_7][1][0];
					// lhs[i_3][j_6][k_7][1][1][1] = 1.0 + tmp1 * 2.0 * njac[i_3][j_6][k_7][1][1] + tmp1 * 2.0 * dy2;
					// lhs[i_3][j_6][k_7][1][1][2] = tmp1 * 2.0 * njac[i_3][j_6][k_7][1][2];
					// lhs[i_3][j_6][k_7][1][1][3] = tmp1 * 2.0 * njac[i_3][j_6][k_7][1][3];
					// lhs[i_3][j_6][k_7][1][1][4] = tmp1 * 2.0 * njac[i_3][j_6][k_7][1][4];
					// lhs[i_3][j_6][k_7][1][2][0] = tmp1 * 2.0 * njac[i_3][j_6][k_7][2][0];
					// lhs[i_3][j_6][k_7][1][2][1] = tmp1 * 2.0 * njac[i_3][j_6][k_7][2][1];
					// lhs[i_3][j_6][k_7][1][2][2] = 1.0 + tmp1 * 2.0 * njac[i_3][j_6][k_7][2][2] + tmp1 * 2.0 * dy3;
					// lhs[i_3][j_6][k_7][1][2][3] = tmp1 * 2.0 * njac[i_3][j_6][k_7][2][3];
					// lhs[i_3][j_6][k_7][1][2][4] = tmp1 * 2.0 * njac[i_3][j_6][k_7][2][4];
					// lhs[i_3][j_6][k_7][1][3][0] = tmp1 * 2.0 * njac[i_3][j_6][k_7][3][0];
					// lhs[i_3][j_6][k_7][1][3][1] = tmp1 * 2.0 * njac[i_3][j_6][k_7][3][1];
					// lhs[i_3][j_6][k_7][1][3][2] = tmp1 * 2.0 * njac[i_3][j_6][k_7][3][2];
					// lhs[i_3][j_6][k_7][1][3][3] = 1.0 + tmp1 * 2.0 * njac[i_3][j_6][k_7][3][3] + tmp1 * 2.0 * dy4;
					// lhs[i_3][j_6][k_7][1][3][4] = tmp1 * 2.0 * njac[i_3][j_6][k_7][3][4];
					// lhs[i_3][j_6][k_7][1][4][0] = tmp1 * 2.0 * njac[i_3][j_6][k_7][4][0];
					// lhs[i_3][j_6][k_7][1][4][1] = tmp1 * 2.0 * njac[i_3][j_6][k_7][4][1];
					// lhs[i_3][j_6][k_7][1][4][2] = tmp1 * 2.0 * njac[i_3][j_6][k_7][4][2];
					// lhs[i_3][j_6][k_7][1][4][3] = tmp1 * 2.0 * njac[i_3][j_6][k_7][4][3];
					// lhs[i_3][j_6][k_7][1][4][4] = 1.0 + tmp1 * 2.0 * njac[i_3][j_6][k_7][4][4] + tmp1 * 2.0 * dy5;

					for (int x1 = 0; x1 <= 4; x1 += 1) {
						for (int x2 = 0; x2 <= 4; x2 += 1) {
							lhs[i_3][j_6][k_7][1][x1][x2] = tmp1 * 2.0 * njac[i_3][j_6][k_7][x1][x2];
						}
					}
					lhs[i_3][j_6][k_7][1][0][0] += 1.0 + tmp1 * 2.0 * dy1;
					lhs[i_3][j_6][k_7][1][1][1] += 1.0 + tmp1 * 2.0 * dy2;
					lhs[i_3][j_6][k_7][1][2][2] += 1.0 + tmp1 * 2.0 * dy3;
					lhs[i_3][j_6][k_7][1][3][3] += 1.0 + tmp1 * 2.0 * dy4;
					lhs[i_3][j_6][k_7][1][4][4] += 1.0 + tmp1 * 2.0 * dy5;

					// lhs[i_3][j_6][k_7][2][0][0] = tmp2 * fjac[i_3][j_6 + 1][k_7][0][0] - tmp1 * njac[i_3][j_6 + 1][k_7][0][0] - tmp1 *
					// dy1; lhs[i_3][j_6][k_7][2][0][1] = tmp2 * fjac[i_3][j_6 + 1][k_7][0][1] - tmp1 * njac[i_3][j_6 + 1][k_7][0][1];
					// lhs[i_3][j_6][k_7][2][0][2] = tmp2 * fjac[i_3][j_6 + 1][k_7][0][2] - tmp1 * njac[i_3][j_6 + 1][k_7][0][2];
					// lhs[i_3][j_6][k_7][2][0][3] = tmp2 * fjac[i_3][j_6 + 1][k_7][0][3] - tmp1 * njac[i_3][j_6 + 1][k_7][0][3];
					// lhs[i_3][j_6][k_7][2][0][4] = tmp2 * fjac[i_3][j_6 + 1][k_7][0][4] - tmp1 * njac[i_3][j_6 + 1][k_7][0][4];
					// lhs[i_3][j_6][k_7][2][1][0] = tmp2 * fjac[i_3][j_6 + 1][k_7][1][0] - tmp1 * njac[i_3][j_6 + 1][k_7][1][0];
					// lhs[i_3][j_6][k_7][2][1][1] = tmp2 * fjac[i_3][j_6 + 1][k_7][1][1] - tmp1 * njac[i_3][j_6 + 1][k_7][1][1] - tmp1 *
					// dy2; lhs[i_3][j_6][k_7][2][1][2] = tmp2 * fjac[i_3][j_6 + 1][k_7][1][2] - tmp1 * njac[i_3][j_6 + 1][k_7][1][2];
					// lhs[i_3][j_6][k_7][2][1][3] = tmp2 * fjac[i_3][j_6 + 1][k_7][1][3] - tmp1 * njac[i_3][j_6 + 1][k_7][1][3];
					// lhs[i_3][j_6][k_7][2][1][4] = tmp2 * fjac[i_3][j_6 + 1][k_7][1][4] - tmp1 * njac[i_3][j_6 + 1][k_7][1][4];
					// lhs[i_3][j_6][k_7][2][2][0] = tmp2 * fjac[i_3][j_6 + 1][k_7][2][0] - tmp1 * njac[i_3][j_6 + 1][k_7][2][0];
					// lhs[i_3][j_6][k_7][2][2][1] = tmp2 * fjac[i_3][j_6 + 1][k_7][2][1] - tmp1 * njac[i_3][j_6 + 1][k_7][2][1];
					// lhs[i_3][j_6][k_7][2][2][2] = tmp2 * fjac[i_3][j_6 + 1][k_7][2][2] - tmp1 * njac[i_3][j_6 + 1][k_7][2][2] - tmp1 *
					// dy3; lhs[i_3][j_6][k_7][2][2][3] = tmp2 * fjac[i_3][j_6 + 1][k_7][2][3] - tmp1 * njac[i_3][j_6 + 1][k_7][2][3];
					// lhs[i_3][j_6][k_7][2][2][4] = tmp2 * fjac[i_3][j_6 + 1][k_7][2][4] - tmp1 * njac[i_3][j_6 + 1][k_7][2][4];
					// lhs[i_3][j_6][k_7][2][3][0] = tmp2 * fjac[i_3][j_6 + 1][k_7][3][0] - tmp1 * njac[i_3][j_6 + 1][k_7][3][0];
					// lhs[i_3][j_6][k_7][2][3][1] = tmp2 * fjac[i_3][j_6 + 1][k_7][3][1] - tmp1 * njac[i_3][j_6 + 1][k_7][3][1];
					// lhs[i_3][j_6][k_7][2][3][2] = tmp2 * fjac[i_3][j_6 + 1][k_7][3][2] - tmp1 * njac[i_3][j_6 + 1][k_7][3][2];
					// lhs[i_3][j_6][k_7][2][3][3] = tmp2 * fjac[i_3][j_6 + 1][k_7][3][3] - tmp1 * njac[i_3][j_6 + 1][k_7][3][3] - tmp1 *
					// dy4; lhs[i_3][j_6][k_7][2][3][4] = tmp2 * fjac[i_3][j_6 + 1][k_7][3][4] - tmp1 * njac[i_3][j_6 + 1][k_7][3][4];
					// lhs[i_3][j_6][k_7][2][4][0] = tmp2 * fjac[i_3][j_6 + 1][k_7][4][0] - tmp1 * njac[i_3][j_6 + 1][k_7][4][0];
					// lhs[i_3][j_6][k_7][2][4][1] = tmp2 * fjac[i_3][j_6 + 1][k_7][4][1] - tmp1 * njac[i_3][j_6 + 1][k_7][4][1];
					// lhs[i_3][j_6][k_7][2][4][2] = tmp2 * fjac[i_3][j_6 + 1][k_7][4][2] - tmp1 * njac[i_3][j_6 + 1][k_7][4][2];
					// lhs[i_3][j_6][k_7][2][4][3] = tmp2 * fjac[i_3][j_6 + 1][k_7][4][3] - tmp1 * njac[i_3][j_6 + 1][k_7][4][3];
					// lhs[i_3][j_6][k_7][2][4][4] = tmp2 * fjac[i_3][j_6 + 1][k_7][4][4] - tmp1 * njac[i_3][j_6 + 1][k_7][4][4] - tmp1 *
					// dy5;

					for (int x1 = 0; x1 <= 4; x1 += 1) {
						for (int x2 = 0; x2 <= 4; x2 += 1) {
							lhs[i_3][j_6][k_7][2][x1][x2] = tmp2 * fjac[i_3][j_6 + 1][k_7][x1][x2] - tmp1 * njac[i_3][j_6 + 1][k_7][x1][x2];
						}
					}
					lhs[i_3][j_6][k_7][2][0][0] -= tmp1 * dy1;
					lhs[i_3][j_6][k_7][2][1][1] -= tmp1 * dy2;
					lhs[i_3][j_6][k_7][2][2][2] -= tmp1 * dy3;
					lhs[i_3][j_6][k_7][2][3][3] -= tmp1 * dy4;
				}
			}
		}
	}

	i = i_3;
	j = j_6;
	k = k_7;
})

/**
 * @brief Run the benchmarks for bt_c_lhsy_line1293_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(bt_c_lhsy_line1293_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}