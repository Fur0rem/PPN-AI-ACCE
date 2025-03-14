/**
 * @file benchmarks/codelet_polybench_lu_c_pintgr_line2019_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) lu_c_pintgr_line2019_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
int ibeg = 1;
int ifin1 = 61;
int j = 0;
int jbeg = 1;
int jfin1 = 60;
double frc1 = 0.000000;
double phi1[64 + 2][64 + 2];
double phi2[64 + 2][64 + 2];

BENCHMARK_FN(lu_c_pintgr_line2019_loop_c_0_c, {
	int __i_0__ = i;
	int __j_1__ = j;

	{
		for (__i_0__ = ibeg; __i_0__ <= ifin1; __i_0__ += 1) {
			for (__j_1__ = jbeg; __j_1__ <= jfin1; __j_1__ += 1) {
				frc1 = frc1 +
					   (phi1[__i_0__][__j_1__] + phi1[__i_0__ + 1][__j_1__] + phi1[__i_0__][__j_1__ + 1] + phi1[__i_0__ + 1][__j_1__ + 1] +
						phi2[__i_0__][__j_1__] + phi2[__i_0__ + 1][__j_1__] + phi2[__i_0__][__j_1__ + 1] + phi2[__i_0__ + 1][__j_1__ + 1]);
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for lu_c_pintgr_line2019_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(lu_c_pintgr_line2019_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}