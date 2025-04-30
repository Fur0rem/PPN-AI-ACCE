/**
 * @file codelets/codelet_polybench_doitgen_c_init_array_line32_0.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 0;
int nr = 150;
int j = 0;
int nq = 140;
int k = 0;
int np = 160;
double a[150 + 0][140 + 0][160 + 0];

BENCHMARK_FN(doitgen_c_init_array_line32_loop_c_0_c, {
	int __i_0__ = i;
	int __j_1__ = j;
	int __k_2__ = k;

	{
		for (__i_0__ = 0; __i_0__ <= nr - 1; __i_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= nq - 1; __j_1__ += 1) {
				for (__k_2__ = 0; __k_2__ <= np - 1; __k_2__ += 1) {
					a[__i_0__][__j_1__][__k_2__] = ((double)((__i_0__ * __j_1__ + __k_2__) % np)) / np;
				}
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
	k = __k_2__;
})

/**
 * @brief Run the benchmarks for doitgen_c_init_array_line32_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(doitgen_c_init_array_line32_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}