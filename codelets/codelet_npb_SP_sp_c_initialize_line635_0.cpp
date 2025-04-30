/**
 * @file codelets/codelet_polybench_sp_c_initialize_line635_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) sp_c_initialize_line635_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
int j = 0;
int k = 0;
double u[5][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];

BENCHMARK_FN(sp_c_initialize_line635_loop_c_0_c, {
	int __i_0__ = i;
	int __j_1__ = j;
	int __k_2__ = k;

	{
		for (__i_0__ = 0; __i_0__ <= 63; __i_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= 63; __j_1__ += 1) {
				for (__k_2__ = 0; __k_2__ <= 63; __k_2__ += 1) {
					u[0][__i_0__][__j_1__][__k_2__] = 1.0;
					u[1][__i_0__][__j_1__][__k_2__] = 0.0;
					u[2][__i_0__][__j_1__][__k_2__] = 0.0;
					u[3][__i_0__][__j_1__][__k_2__] = 0.0;
					u[4][__i_0__][__j_1__][__k_2__] = 1.0;
				}
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
	k = __k_2__;
})

/**
 * @brief Run the benchmarks for sp_c_initialize_line635_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(sp_c_initialize_line635_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}