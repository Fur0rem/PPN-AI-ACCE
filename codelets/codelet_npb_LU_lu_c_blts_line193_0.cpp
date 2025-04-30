/**
 * @file codelets/codelet_polybench_lu_c_blts_line193_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) lu_c_blts_line193_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
int ist = 1;
int iend = 62;
int j = 0;
int jst = 1;
int jend = 62;
int m = 0;
double v[64][64 / 2 * 2 + 1][64 / 2 * 2 + 1][5];
int k = 49;
double omega = 1.200000;
double ldz[64][64][5][5];

BENCHMARK_FN(lu_c_blts_line193_loop_c_0_c, {
	int __i_0__ = i;
	int __j_1__ = j;
	int __m_2__ = m;

	{
		for (__i_0__ = ist; __i_0__ <= iend; __i_0__ += 1) {
			for (__j_1__ = jst; __j_1__ <= jend; __j_1__ += 1) {
				for (__m_2__ = 0; __m_2__ <= 4; __m_2__ += 1) {
					v[__i_0__][__j_1__][k][__m_2__] =
						v[__i_0__][__j_1__][k][__m_2__] - omega * (ldz[__i_0__][__j_1__][__m_2__][0] * v[__i_0__][__j_1__][k - 1][0] +
																   ldz[__i_0__][__j_1__][__m_2__][1] * v[__i_0__][__j_1__][k - 1][1] +
																   ldz[__i_0__][__j_1__][__m_2__][2] * v[__i_0__][__j_1__][k - 1][2] +
																   ldz[__i_0__][__j_1__][__m_2__][3] * v[__i_0__][__j_1__][k - 1][3] +
																   ldz[__i_0__][__j_1__][__m_2__][4] * v[__i_0__][__j_1__][k - 1][4]);
				}
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
	m = __m_2__;
})

/**
 * @brief Run the benchmarks for lu_c_blts_line193_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(lu_c_blts_line193_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}