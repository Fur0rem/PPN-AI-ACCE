/**
 * @file benchmarks/codelet_serial_transpose_c_main_line176_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) Kernels serial codelet
 */

#include "bench_common.hpp"

int order = 2000;
double abserr = 0.000000;
double* b_p = new double[2000 * 2000];

BENCHMARK_FN(transpose_c_main_line176_loop_c_0_c, {
	for (int j = 0; j <= order - 1; j += 1) {
		for (int i = 0; i <= order - 1; i += 1) {
			abserr += (b_p[i + order * j] - ((double)(order * i + j)) >= 0 ? b_p[i + order * j] - ((double)(order * i + j))
																		   : -(b_p[i + order * j] - ((double)(order * i + j))));
		}
	}
})

/**
 * @brief Run the benchmarks for transpose_c_main_line176_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(transpose_c_main_line176_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
