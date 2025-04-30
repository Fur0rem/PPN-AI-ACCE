/**
 * @file codelets/codelet_nn_backpropagate_second_dervatives_line262.cpp
 * @brief Benchmark for backpropagate second derivatives for the neural network
 */

#include "bench_common.hpp"

constexpr int MAX_COL = 1250;
constexpr int I_SIZE = 5;
double differentials[I_SIZE][MAX_COL];

/**
 * @brief Perform the backpropagate second derivatives for the neural network
 */
BENCHMARK_FN(codelet_nn_backpropagate_second_dervatives_line262, {
	for (int i = 0; i <= I_SIZE - 1; i += 1) {
		for (int j = 0; j <= MAX_COL - 1; j += 1) {
			differentials[i][j] = 0.0;
		}
	}
})

/**
 * @brief Run the benchmarks for dummy
 */
int main() {
	std::ostringstream oss;
	auto result =
		BENCHMARK_INIT(1000, oss).RUN(codelet_nn_backpropagate_second_dervatives_line262).doNotOptimizeAway(differentials).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
