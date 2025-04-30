/**
 * @file codelets/codelet_polybench_sp_c_lhsz_line1142_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) sp_c_lhsz_line1142_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
int grid_points[3] = {64, 64, 64};
int j = 0;
double lhs[15][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
int k = 61;
double comz1 = 0.000375;
double comz4 = 0.001500;
double comz6 = 0.002250;
double comz5 = 0.001875;

BENCHMARK_FN(sp_c_lhsz_line1142_loop_c_0_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[1];
	int __i_2__ = i;
	int __j_3__ = j;

	{
		for (__i_2__ = 1; __i_2__ <= __arr_sub_0__ - 2; __i_2__ += 1) {
			for (__j_3__ = 1; __j_3__ <= __arr_sub_1__ - 2; __j_3__ += 1) {
				lhs[0][__i_2__][__j_3__][k] = lhs[0][__i_2__][__j_3__][k] + comz1;
				lhs[1][__i_2__][__j_3__][k] = lhs[1][__i_2__][__j_3__][k] - comz4;
				lhs[2][__i_2__][__j_3__][k] = lhs[2][__i_2__][__j_3__][k] + comz6;
				lhs[3][__i_2__][__j_3__][k] = lhs[3][__i_2__][__j_3__][k] - comz4;
				lhs[0][__i_2__][__j_3__][k + 1] = lhs[0][__i_2__][__j_3__][k + 1] + comz1;
				lhs[1][__i_2__][__j_3__][k + 1] = lhs[1][__i_2__][__j_3__][k + 1] - comz4;
				lhs[2][__i_2__][__j_3__][k + 1] = lhs[2][__i_2__][__j_3__][k + 1] + comz5;
			}
		}
	}

	i = __i_2__;
	j = __j_3__;
})

/**
 * @brief Run the benchmarks for sp_c_lhsz_line1142_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(sp_c_lhsz_line1142_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}