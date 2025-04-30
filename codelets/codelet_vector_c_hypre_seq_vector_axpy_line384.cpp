/**
 * @file codelets/codelet_vector_c_hypre_seq_vector_axpy_line384.cpp
 * @brief Benchmark for axpy operation on vectors
 */

#include "bench_common.hpp"

constexpr int SIZE = 125000;
double x_data[SIZE] = {3.0};
double y_data[SIZE] = {2.0};
double alpha = 0.5;

/**
 * @brief Do the axpy operation on vectors
 */
BENCHMARK_FN(codelet_vector_c_hypre_seq_vector_axpy_line384, {
	for (int i = 0; i <= SIZE - 1; i += 1) {
		y_data[i] += alpha * x_data[i];
	}
})

/**
 * @brief Run the benchmarks for codelet_generate_seq_laplacian_line73
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(1000, oss)
					  .RUN(codelet_vector_c_hypre_seq_vector_axpy_line384)
					  .doNotOptimizeAway(y_data)
					  .doNotOptimizeAway(x_data)
					  .doNotOptimizeAway(alpha)
					  .results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
