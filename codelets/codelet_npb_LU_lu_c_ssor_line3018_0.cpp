/**
 * @file codelets/codelet_polybench_lu_c_ssor_line3018_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) lu_c_ssor_line3018_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
int ist = 1;
int iend = 62;
int j = 0;
int jst = 1;
int jend = 62;
int k = 0;
int nz = 64;
int m = 0;
double rsd[64][64 / 2 * 2 + 1][64 / 2 * 2 + 1][5];
double dt = 2.000000;

BENCHMARK_FN(lu_c_ssor_line3018_loop_c_0_c, {
	int __i_0__ = i;
	int __j_1__ = j;
	int __k_2__ = k;
	int __m_3__ = m;

	{
		for (__i_0__ = ist; __i_0__ <= iend; __i_0__ += 1) {
			for (__j_1__ = jst; __j_1__ <= jend; __j_1__ += 1) {
				for (__k_2__ = 1; __k_2__ <= nz - 2; __k_2__ += 1) {
					for (__m_3__ = 0; __m_3__ <= 4; __m_3__ += 1) {
						rsd[__i_0__][__j_1__][__k_2__][__m_3__] = dt * rsd[__i_0__][__j_1__][__k_2__][__m_3__];
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
 * @brief Run the benchmarks for lu_c_ssor_line3018_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(lu_c_ssor_line3018_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}