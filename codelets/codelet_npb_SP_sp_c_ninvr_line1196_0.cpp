/**
 * @file codelets/codelet_polybench_sp_c_ninvr_line1196_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) sp_c_ninvr_line1196_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
int grid_points[3] = {64, 64, 64};
int j = 0;
int k = 0;
double r1 = 0;
double rhs[5][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double r2 = 0;
double r3 = 0;
double r4 = 0;
double r5 = 0;
double t1 = 0;
double bt = 0.707107;
double t2 = 0;

BENCHMARK_FN(sp_c_ninvr_line1196_loop_c_0_c, {
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
					r1 = rhs[0][__i_3__][__j_4__][__k_5__];
					r2 = rhs[1][__i_3__][__j_4__][__k_5__];
					r3 = rhs[2][__i_3__][__j_4__][__k_5__];
					r4 = rhs[3][__i_3__][__j_4__][__k_5__];
					r5 = rhs[4][__i_3__][__j_4__][__k_5__];
					t1 = bt * r3;
					t2 = 0.5 * (r4 + r5);
					rhs[0][__i_3__][__j_4__][__k_5__] = -r2;
					rhs[1][__i_3__][__j_4__][__k_5__] = r1;
					rhs[2][__i_3__][__j_4__][__k_5__] = bt * (r4 - r5);
					rhs[3][__i_3__][__j_4__][__k_5__] = -t1 + t2;
					rhs[4][__i_3__][__j_4__][__k_5__] = t1 + t2;
				}
			}
		}
	}

	i = __i_3__;
	j = __j_4__;
	k = __k_5__;
})

/**
 * @brief Run the benchmarks for sp_c_ninvr_line1196_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(sp_c_ninvr_line1196_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}