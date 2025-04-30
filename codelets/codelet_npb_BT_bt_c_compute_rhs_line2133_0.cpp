/**
 * @file codelets/codelet_polybench_bt_c_compute_rhs_line2133_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) bt_c_compute_rhs_line2133_0 codelet
 */

#include "bench_common.hpp"

int i = 63;
int grid_points[3] = {64, 64, 64};
int j = 63;
int k = 2;
int m = 5;
double rhs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1][5];
double dssp = 0.250000;
double u[(64 + 1) / 2 * 2 + 1][(64 + 1) / 2 * 2 + 1][(64 + 1) / 2 * 2 + 1][5];

void ___bt_c_compute_rhs_line2133_loop_c_0_c_start_PPN_LABEL() {
	int arr_sub_0 = grid_points[0];
	int arr_sub_1 = grid_points[1];
	int arr_sub_2 = grid_points[2];
	int i_3 = i;
	int j_4 = j;
	int k_5 = k;
	int m_6 = m;

	NO_VECTORISE_NO_UNROLL
	for (i_3 = 1; i_3 <= arr_sub_0 - 1 - 1; i_3 += 1) {
		NO_VECTORISE_NO_UNROLL
		for (j_4 = 1; j_4 <= arr_sub_1 - 1 - 1; j_4 += 1) {
			NO_VECTORISE_NO_UNROLL
			for (k_5 = 3; k_5 <= arr_sub_2 - 3 - 1; k_5 += 1) {
				NO_VECTORISE_NO_UNROLL
				for (m_6 = 0; m_6 <= 4; m_6 += 1) {
					rhs[i_3][j_4][k_5][m_6] =
						rhs[i_3][j_4][k_5][m_6] -
						dssp * (u[i_3][j_4][k_5 - 2][m_6] - 4.0 * u[i_3][j_4][k_5 - 1][m_6] + 6.0 * u[i_3][j_4][k_5][m_6] -
								4.0 * u[i_3][j_4][k_5 + 1][m_6] + u[i_3][j_4][k_5 + 2][m_6]);
				}
			}
		}
	}

	i = i_3;
	j = j_4;
	k = k_5;
	m = m_6;
}

/**
 * @brief Run the benchmarks for bt_c_compute_rhs_line2133_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(bt_c_compute_rhs_line2133_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}