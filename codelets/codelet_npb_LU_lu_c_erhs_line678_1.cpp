/**
 * @file codelets/codelet_polybench_lu_c_erhs_line678_1.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) lu_c_erhs_line678_1 codelet
 */

#include "bench_common.hpp"

int i = 0;
int nx = 64;
int j = 0;
int ny = 64;
int k = 0;
int nz = 64;
int m = 0;
double frct[64][64 / 2 * 2 + 1][64 / 2 * 2 + 1][5];

BENCHMARK_FN(lu_c_erhs_line678_loop_c_1_c, {
	int __i_0__ = i;
	int __j_1__ = j;
	int __k_2__ = k;
	int __m_3__ = m;

	{
		for (__i_0__ = 0; __i_0__ <= nx - 1; __i_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= ny - 1; __j_1__ += 1) {
				for (__k_2__ = 0; __k_2__ <= nz - 1; __k_2__ += 1) {
					/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
					/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
					int _lu_fringe_1 = 2;
					for (__m_3__ = 0; __m_3__ <= 4 - _lu_fringe_1; __m_3__ += 2) {
						frct[__i_0__][__j_1__][__k_2__][__m_3__] = 0.0;
						{ frct[__i_0__][__j_1__][__k_2__][__m_3__ + 1] = 0.0; }
					}
					for (; __m_3__ <= 4; __m_3__ += 1) {
						frct[__i_0__][__j_1__][__k_2__][__m_3__] = 0.0;
					}
				}
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
	k = __k_2__;
	m = __m_3__;
})

/**
 * @brief Run the benchmarks for lu_c_erhs_line678_loop_c_1_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(lu_c_erhs_line678_loop_c_1_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}