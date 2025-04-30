/**
 * @file codelets/codelet_libraries_default_dct_c_compute_mse_line8497_0.cpp
 * @brief Benchmark extracted from the Libraries default codelet
 */

#include "bench_common.hpp"

int i = 0;
int n = 32;
int j = 0;
double ret = 0.000000;
double e[1 << 6][1 << 6];
double basis[1 << 2 + 5 - 1][1 << 2 + 5 - 1];
double tbasis[1 << 2 + 5 - 1][1 << 2 + 5 - 1];

OPTIMISE_SIZE
BENCHMARK_FN(dct_c_compute_mse_line8497_loop_c_0_c, {
	int __i_0__ = i;
	int __j_1__ = j;

	{
		for (__i_0__ = 0; __i_0__ <= n - 1; __i_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= n - 1; __j_1__ += 1) {
				ret += e[__i_0__][__j_1__] * (basis[__i_0__][__j_1__] - tbasis[__i_0__][__j_1__]);
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for dct_c_compute_mse_line8497_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(dct_c_compute_mse_line8497_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
