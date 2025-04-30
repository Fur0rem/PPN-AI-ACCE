/**
 * @file codelets/codelet_libraries_default_alac_c_alac_read_d_line575_0.cpp
 * @brief Benchmark extracted from the Libraries default codelet
 */

#include "bench_common.hpp"

int k = 0;
int readcount = 4096;
long total = 4096;
double normfact = 0.000000;
typedef long int64_t;
typedef int64_t sf_count_t;
double* ptr = new double[8192];
int* iptr = new int[4096];

BENCHMARK_FN(alac_c_alac_read_d_line575_loop_c_0_c, {
	int __k_0__ = k;

	{
		for (__k_0__ = 0; __k_0__ <= readcount - 1; __k_0__ += 1) {
			ptr[total + __k_0__] = normfact * iptr[__k_0__];
		}
	}

	k = __k_0__;
})

/**
 * @brief Run the benchmarks for alac_c_alac_read_d_line575_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(alac_c_alac_read_d_line575_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
