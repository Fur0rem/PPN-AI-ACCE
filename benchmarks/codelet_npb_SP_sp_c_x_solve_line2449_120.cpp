/**
 * @file benchmarks/codelet_polybench_sp_c_x_solve_line2449_120.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) sp_c_x_solve_line2449_120 codelet
 */

#include "bench_common.hpp"

int m = 0;
int j = 0;
int grid_points[3] = {64, 64, 64};
int k = 0;
double rhs[5][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
int i = 62;
double lhs[15][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
int n = 0;
int i1 = 63;

BENCHMARK_FN(sp_c_x_solve_line2449_loop_c_120_c, {
	int __arr_sub_0__ = grid_points[1];
	int __arr_sub_1__ = grid_points[2];
	int __m_2__ = m;
	int __j_3__ = j;
	int __k_4__ = k;

	{
		for (__j_3__ = 1; __j_3__ <= __arr_sub_0__ - 2; __j_3__ += 1) {
			for (__k_4__ = 1; __k_4__ <= __arr_sub_1__ - 2; __k_4__ += 1) {
				for (__m_2__ = 0; __m_2__ <= 2; __m_2__ += 1) {
					rhs[__m_2__][i][__j_3__][__k_4__] =
						rhs[__m_2__][i][__j_3__][__k_4__] - lhs[n + 3][i][__j_3__][__k_4__] * rhs[__m_2__][i1][__j_3__][__k_4__];
				}
			}
		}
	}

	m = __m_2__;
	j = __j_3__;
	k = __k_4__;
})

/**
 * @brief Run the benchmarks for sp_c_x_solve_line2449_loop_c_120_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(sp_c_x_solve_line2449_loop_c_120_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}