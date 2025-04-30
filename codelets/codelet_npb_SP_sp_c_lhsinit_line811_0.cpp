/**
 * @file codelets/codelet_polybench_sp_c_lhsinit_line811_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) sp_c_lhsinit_line811_0 codelet
 */

#include "bench_common.hpp"

int n = 0;
int i = 0;
int grid_points[3] = {64, 64, 64};
int j = 0;
int k = 0;
double lhs[15][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];

BENCHMARK_FN(sp_c_lhsinit_line811_loop_c_0_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[1];
	int __arr_sub_2__ = grid_points[2];
	int __n_3__ = n;
	int __i_4__ = i;
	int __j_5__ = j;
	int __k_6__ = k;

	{
		for (__n_3__ = 0; __n_3__ <= 14; __n_3__ += 1) {
			for (__i_4__ = 0; __i_4__ <= __arr_sub_0__ - 1; __i_4__ += 1) {
				for (__j_5__ = 0; __j_5__ <= __arr_sub_1__ - 1; __j_5__ += 1) {
					for (__k_6__ = 0; __k_6__ <= __arr_sub_2__ - 1; __k_6__ += 1) {
						lhs[__n_3__][__i_4__][__j_5__][__k_6__] = 0.0;
					}
				}
			}
		}
	}

	n = __n_3__;
	i = __i_4__;
	j = __j_5__;
	k = __k_6__;
})

/**
 * @brief Run the benchmarks for sp_c_lhsinit_line811_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(sp_c_lhsinit_line811_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}