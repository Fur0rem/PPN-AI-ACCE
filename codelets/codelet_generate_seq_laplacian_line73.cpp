/**
 * @file codelets/codelet_generate_seq_laplacian_line73.cpp
 * @brief Benchmark for codelet_generate_seq_laplacian_line73
 */

#include "bench_common.hpp"

constexpr int GRID_SIZE = 125000;
double x_data[GRID_SIZE];
double sol_data[GRID_SIZE];
double rhs_data[GRID_SIZE];

/**
 * @brief Generate the laplacian
 */
BENCHMARK_FN(codelet_generate_seq_laplacian_line73, {
	for (int i = 0; i <= GRID_SIZE - 1; i += 1) {
		x_data[i] = 0.0;
		sol_data[i] = 0.0;
		rhs_data[i] = 1.0;
	}
})

/**
 * @brief Run the benchmarks for codelet_generate_seq_laplacian_line73
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(1000, oss)
					  .RUN(codelet_generate_seq_laplacian_line73)
					  .doNotOptimizeAway(x_data)
					  .doNotOptimizeAway(sol_data)
					  .doNotOptimizeAway(rhs_data)
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
