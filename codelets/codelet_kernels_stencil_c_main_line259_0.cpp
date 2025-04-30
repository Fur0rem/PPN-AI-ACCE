/**
 * @file codelets/codelet_serial_stencil_c_main_line259_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) Kernels stencil codelet
 */

#include "bench_common.hpp"

long n = 2000;
float norm = 0.000000;
float* out = new float[n * n];

BENCHMARK_FN(stencil_c_main_line259_loop_c_0_c, {
	for (int j = 2; ((long)j) <= n - ((long)2) - 1; j += 1) {
		for (int i = 2; ((long)i) <= n - ((long)2) - 1; i += 1) {
			norm += ((float)((out[i + j * n] >= 0 ? out[i + j * n] : -out[i + j * n])));
		}
	}
})

/**
 * @brief Run the benchmarks for stencil_c_main_line259_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(stencil_c_main_line259_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
