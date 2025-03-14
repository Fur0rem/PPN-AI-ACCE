/**
 * @file benchmarks/codelet_polybench_sp_c_lhsy_line1014_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) sp_c_lhsy_line1014_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
int grid_points[3] = {64, 64, 64};
int j = 0;
int k = 0;
double lhs[15][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double comz1 = 0.000375;
double comz4 = 0.001500;
double comz6 = 0.002250;

BENCHMARK_FN(sp_c_lhsy_line1014_loop_c_0_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[1];
	int __arr_sub_2__ = grid_points[2];
	int __i_3__ = i;
	int __j_4__ = j;
	int __k_5__ = k;

	{
		for (__i_3__ = 1; __i_3__ <= __arr_sub_0__ - 2; __i_3__ += 1) {
			for (__j_4__ = 3; __j_4__ <= __arr_sub_1__ - 4; __j_4__ += 1) {
				for (__k_5__ = 1; __k_5__ <= __arr_sub_2__ - 2; __k_5__ += 1) {
					lhs[0][__i_3__][__j_4__][__k_5__] = lhs[0][__i_3__][__j_4__][__k_5__] + comz1;
					lhs[1][__i_3__][__j_4__][__k_5__] = lhs[1][__i_3__][__j_4__][__k_5__] - comz4;
					lhs[2][__i_3__][__j_4__][__k_5__] = lhs[2][__i_3__][__j_4__][__k_5__] + comz6;
					lhs[3][__i_3__][__j_4__][__k_5__] = lhs[3][__i_3__][__j_4__][__k_5__] - comz4;
					lhs[4][__i_3__][__j_4__][__k_5__] = lhs[4][__i_3__][__j_4__][__k_5__] + comz1;
				}
			}
		}
	}

	i = __i_3__;
	j = __j_4__;
	k = __k_5__;
})

/**
 * @brief Run the benchmarks for sp_c_lhsy_line1014_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(sp_c_lhsy_line1014_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}