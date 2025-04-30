/**
 * @file codelets/codelet_polybench_lu_c_l2norm_line1921_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) lu_c_l2norm_line1921_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
int ist = 1;
int iend = 62;
int j = 0;
int jst = 1;
int jend = 62;
int k = 0;
int nz0 = 64;
double sum0 = 0.000000;
double v[64][64 / 2 * 2 + 1][64 / 2 * 2 + 1][5];
double sum1 = 0.000000;
double sum2 = 0.000000;
double sum3 = 0.000000;
double sum4 = 0.000000;

BENCHMARK_FN(lu_c_l2norm_line1921_loop_c_0_c, {
	int __i_0__ = i;
	int __j_1__ = j;
	int __k_2__ = k;

	{
		for (__i_0__ = ist; __i_0__ <= iend; __i_0__ += 1) {
			for (__j_1__ = jst; __j_1__ <= jend; __j_1__ += 1) {
				for (__k_2__ = 1; __k_2__ <= nz0 - 2; __k_2__ += 1) {
					sum0 = sum0 + v[__i_0__][__j_1__][__k_2__][0] * v[__i_0__][__j_1__][__k_2__][0];
					sum1 = sum1 + v[__i_0__][__j_1__][__k_2__][1] * v[__i_0__][__j_1__][__k_2__][1];
					sum2 = sum2 + v[__i_0__][__j_1__][__k_2__][2] * v[__i_0__][__j_1__][__k_2__][2];
					sum3 = sum3 + v[__i_0__][__j_1__][__k_2__][3] * v[__i_0__][__j_1__][__k_2__][3];
					sum4 = sum4 + v[__i_0__][__j_1__][__k_2__][4] * v[__i_0__][__j_1__][__k_2__][4];
				}
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
	k = __k_2__;
})

/**
 * @brief Run the benchmarks for lu_c_l2norm_line1921_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(lu_c_l2norm_line1921_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}