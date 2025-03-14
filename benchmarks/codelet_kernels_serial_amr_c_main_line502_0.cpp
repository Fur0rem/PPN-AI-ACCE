/**
 * @file benchmarks/codelet_serial_amr_c_main_line502_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) UA-NSU codelet
 */

#include "bench_common.hpp"

long j = 0;
long n = 512;
long i = 0;
double norm = 0.000000;
double* out = new double[n * n];

BENCHMARK_FN(amr_c_main_line502_loop_c_0_c, {
	int __j_0__ = j;
	int __i_1__ = i;

	for (__j_0__ = 2; __j_0__ <= n - ((long)2) - 1; __j_0__ += 1) {
		for (__i_1__ = 2; __i_1__ <= n - ((long)2) - 1; __i_1__ += 1) {
			norm += ((double)((out[__i_1__ + __j_0__ * n] >= 0 ? out[__i_1__ + __j_0__ * n] : -out[__i_1__ + __j_0__ * n])));
		}
	}

	j = __j_0__;
	i = __i_1__;
})

/**
 * @brief Run the benchmarks for amr_c_main_line502_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(amr_c_main_line502_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
