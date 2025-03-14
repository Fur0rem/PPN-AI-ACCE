/**
 * @file benchmarks/codelet_polybench_lu_c_pintgr_line2128_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) lu_c_pintgr_line2128_0 codelet
 */

#include "bench_common.hpp"

int j = 0;
int jbeg = 1;
int jfin1 = 60;
int k = 0;
int ki1 = 2;
int ki2 = 62;
double frc3 = 0.000000;
double phi1[64 + 2][64 + 2];
double phi2[64 + 2][64 + 2];

BENCHMARK_FN(lu_c_pintgr_line2128_loop_c_0_c, {
	int __j_0__ = j;
	int __k_1__ = k;

	{
		for (__j_0__ = jbeg; __j_0__ <= jfin1; __j_0__ += 1) {
			for (__k_1__ = ki1; __k_1__ <= ki2 - 1; __k_1__ += 1) {
				frc3 = frc3 +
					   (phi1[__j_0__][__k_1__] + phi1[__j_0__ + 1][__k_1__] + phi1[__j_0__][__k_1__ + 1] + phi1[__j_0__ + 1][__k_1__ + 1] +
						phi2[__j_0__][__k_1__] + phi2[__j_0__ + 1][__k_1__] + phi2[__j_0__][__k_1__ + 1] + phi2[__j_0__ + 1][__k_1__ + 1]);
			}
		}
	}

	j = __j_0__;
	k = __k_1__;
})

/**
 * @brief Run the benchmarks for lu_c_pintgr_line2128_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(lu_c_pintgr_line2128_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}