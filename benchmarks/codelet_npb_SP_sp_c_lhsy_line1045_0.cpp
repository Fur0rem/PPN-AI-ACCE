/**
 * @file benchmarks/codelet_polybench_sp_c_lhsy_line1045_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) sp_c_lhsy_line1045_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
int grid_points[3] = {64, 64, 64};
int j = 0;
int k = 0;
double lhs[15][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double dtty2 = 0.047250;
double speed[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];

OPTIMISE_SIZE
BENCHMARK_FN(sp_c_lhsy_line1045_loop_c_0_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[1];
	int __arr_sub_2__ = grid_points[2];
	int __i_3__ = i;
	int __j_4__ = j;
	int __k_5__ = k;

	{
		for (__i_3__ = 1; __i_3__ <= __arr_sub_0__ - 2; __i_3__ += 1) {
			for (__j_4__ = 1; __j_4__ <= __arr_sub_1__ - 2; __j_4__ += 1) {
				for (__k_5__ = 1; __k_5__ <= __arr_sub_2__ - 2; __k_5__ += 1) {
					lhs[0 + 5][__i_3__][__j_4__][__k_5__] = lhs[0][__i_3__][__j_4__][__k_5__];
					lhs[1 + 5][__i_3__][__j_4__][__k_5__] =
						lhs[1][__i_3__][__j_4__][__k_5__] - dtty2 * speed[__i_3__][__j_4__ - 1][__k_5__];
					lhs[2 + 5][__i_3__][__j_4__][__k_5__] = lhs[2][__i_3__][__j_4__][__k_5__];
					lhs[3 + 5][__i_3__][__j_4__][__k_5__] =
						lhs[3][__i_3__][__j_4__][__k_5__] + dtty2 * speed[__i_3__][__j_4__ + 1][__k_5__];
					lhs[4 + 5][__i_3__][__j_4__][__k_5__] = lhs[4][__i_3__][__j_4__][__k_5__];
					lhs[0 + '\n'][__i_3__][__j_4__][__k_5__] = lhs[0][__i_3__][__j_4__][__k_5__];
					lhs[1 + '\n'][__i_3__][__j_4__][__k_5__] =
						lhs[1][__i_3__][__j_4__][__k_5__] + dtty2 * speed[__i_3__][__j_4__ - 1][__k_5__];
					lhs[2 + '\n'][__i_3__][__j_4__][__k_5__] = lhs[2][__i_3__][__j_4__][__k_5__];
					lhs[3 + '\n'][__i_3__][__j_4__][__k_5__] =
						lhs[3][__i_3__][__j_4__][__k_5__] - dtty2 * speed[__i_3__][__j_4__ + 1][__k_5__];
					lhs[4 + '\n'][__i_3__][__j_4__][__k_5__] = lhs[4][__i_3__][__j_4__][__k_5__];
				}
			}
		}
	}

	i = __i_3__;
	j = __j_4__;
	k = __k_5__;
})

/**
 * @brief Run the benchmarks for sp_c_lhsy_line1045_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(sp_c_lhsy_line1045_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}