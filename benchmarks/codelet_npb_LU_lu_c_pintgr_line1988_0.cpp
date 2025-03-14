/**
 * @file benchmarks/codelet_polybench_lu_c_pintgr_line1988_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) lu_c_pintgr_line1988_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
int k = 0;
double phi1[64 + 2][64 + 2];
double phi2[64 + 2][64 + 2];

BENCHMARK_FN(lu_c_pintgr_line1988_loop_c_0_c, {
	int __i_0__ = i;
	int __k_1__ = k;

	{
		for (__i_0__ = 0; __i_0__ <= 65; __i_0__ += 1) {
			for (__k_1__ = 0; __k_1__ <= 65; __k_1__ += 1) {
				phi1[__i_0__][__k_1__] = 0.0;
				phi2[__i_0__][__k_1__] = 0.0;
			}
		}
	}

	i = __i_0__;
	k = __k_1__;
})

/**
 * @brief Run the benchmarks for lu_c_pintgr_line1988_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(lu_c_pintgr_line1988_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}