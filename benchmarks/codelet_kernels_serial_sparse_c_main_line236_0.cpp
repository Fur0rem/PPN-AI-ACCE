/**
 * @file benchmarks/codelet_serial_sparse_c_main_line236_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) Kernels serial codelet
 */

#include "bench_common.hpp"

long long row = 0;
long long size2 = 1048576;
typedef long long s64Int;
double vector[1048576];

BENCHMARK_FN(sparse_c_main_line236_loop_c_0_c, {
	int __row_0__ = row;

	{
		for (__row_0__ = 0; __row_0__ <= size2 - 1; __row_0__ += 1) {
			vector[__row_0__] += ((double)(__row_0__ + 1));
		}
	}

	row = __row_0__;
})

/**
 * @brief Run the benchmarks for sparse_c_main_line236_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(sparse_c_main_line236_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
