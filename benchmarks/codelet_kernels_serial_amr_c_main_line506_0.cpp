/**
 * @file benchmarks/codelet_serial_amr_c_main_line506_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) UA-NSU codelet
 */

#include "bench_common.hpp"

long j = 0;
long n = 512;
long i = 0;
double norm_in = 0.000000;
double* in = new double[n * n];

BENCHMARK_FN(amr_c_main_line506_loop_c_0_c, {
	int __j_0__ = j;
	int __i_1__ = i;

	{
		for (__j_0__ = 0; __j_0__ <= n - 1; __j_0__ += 1) {
			for (__i_1__ = 0; __i_1__ <= n - 1; __i_1__ += 1) {
				norm_in += ((double)((in[__i_1__ + __j_0__ * n] >= 0 ? in[__i_1__ + __j_0__ * n] : -in[__i_1__ + __j_0__ * n])));
			}
		}
	}

	j = __j_0__;
	i = __i_1__;
})

/**
 * @brief Run the benchmarks for amr_c_main_line506_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(amr_c_main_line506_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
