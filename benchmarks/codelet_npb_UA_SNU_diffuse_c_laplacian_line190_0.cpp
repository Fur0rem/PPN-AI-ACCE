/**
 * @file benchmarks/codelet_polybench_diffuse_c_laplacian_line190_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) diffuse_c_laplacian_line190_0 codelet
 */

#include "bench_common.hpp"

int iz = 0;
int k = 0;
int j = 0;
int i = 0;
double tm1[5][5][5];
double wdtdr[5][5];
double u[5][5][5];

OPTIMISE_SIZE
BENCHMARK_FN(diffuse_c_laplacian_line190_loop_c_0_c, {
	int __iz_0__ = iz;
	int __k_1__ = k;
	int __j_2__ = j;
	int __i_3__ = i;

	{
		for (__iz_0__ = 0; __iz_0__ <= 4; __iz_0__ += 1) {
			for (__k_1__ = 0; __k_1__ <= 4; __k_1__ += 1) {
				for (__j_2__ = 0; __j_2__ <= 4; __j_2__ += 1) {
					for (__i_3__ = 0; __i_3__ <= 4; __i_3__ += 1) {
						tm1[__iz_0__][__j_2__][__i_3__] =
							tm1[__iz_0__][__j_2__][__i_3__] + wdtdr[__k_1__][__i_3__] * u[__iz_0__][__j_2__][__k_1__];
					}
				}
			}
		}
	}

	iz = __iz_0__;
	k = __k_1__;
	j = __j_2__;
	i = __i_3__;
})

/**
 * @brief Run the benchmarks for diffuse_c_laplacian_line190_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(diffuse_c_laplacian_line190_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}