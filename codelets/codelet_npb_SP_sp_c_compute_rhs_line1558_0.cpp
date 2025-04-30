/**
 * @file codelets/codelet_polybench_sp_c_compute_rhs_line1558_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) sp_c_compute_rhs_line1558_0 codelet
 */

#include "bench_common.hpp"

int m = 0;
int i = 0;
int grid_points[3] = {64, 64, 64};
int k = 0;
double rhs[5][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
int j = 62;
double dssp = 0.250000;
double u[5][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];

BENCHMARK_FN(sp_c_compute_rhs_line1558_loop_c_0_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[2];
	int m2 = m;
	int i3 = i;
	int k4 = k;

	{
		NO_VECTORISE_NO_UNROLL
		for (m2 = 0; m2 <= 4; m2 += 1) {
			NO_VECTORISE_NO_UNROLL
			for (i3 = 1; i3 <= __arr_sub_0__ - 2; i3 += 1) {
				NO_VECTORISE_NO_UNROLL
				for (k4 = 1; k4 <= __arr_sub_1__ - 2; k4 += 1) {
					rhs[m2][i3][j][k4] =
						rhs[m2][i3][j][k4] - dssp * (u[m2][i3][j - 2][k4] - 4.0 * u[m2][i3][j - 1][k4] + 5.0 * u[m2][i3][j][k4]);
				}
			}
		}
	}

	m = m2;
	i = i3;
	k = k4;
})

/**
 * @brief Run the benchmarks for sp_c_compute_rhs_line1558_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(sp_c_compute_rhs_line1558_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}