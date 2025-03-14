/**
 * @file benchmarks/codelet_polybench_lu_c_ssor_line2976_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) lu_c_ssor_line2976_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
int j = 0;
int k = 0;
int m = 0;
double a[64][64][5][5];
double b[64][64][5][5];
double c[64][64][5][5];
double d[64][64][5][5];

BENCHMARK_FN(lu_c_ssor_line2976_loop_c_0_c, {
	int __i_0__ = i;
	int __j_1__ = j;
	int __k_2__ = k;
	int __m_3__ = m;

	{
		for (__i_0__ = 0; __i_0__ <= 63; __i_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= 63; __j_1__ += 1) {
				for (__k_2__ = 0; __k_2__ <= 4; __k_2__ += 1) {
					for (__m_3__ = 0; __m_3__ <= 4; __m_3__ += 1) {
						a[__i_0__][__j_1__][__k_2__][__m_3__] = 0.0;
						b[__i_0__][__j_1__][__k_2__][__m_3__] = 0.0;
						c[__i_0__][__j_1__][__k_2__][__m_3__] = 0.0;
						d[__i_0__][__j_1__][__k_2__][__m_3__] = 0.0;
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
 * @brief Run the benchmarks for lu_c_ssor_line2976_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(lu_c_ssor_line2976_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}