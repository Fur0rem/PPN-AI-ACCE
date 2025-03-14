/**
 * @file benchmarks/codelet_rbm_train.cpp
 * @brief Benchmark of codelet for training Restricted Boltzmann Machine
 */

#include "bench_common.hpp"

double edges[100 * 5 + 1][3 * 100 + 1];
int pos[500 + 1][300 + 1];
int neg[500 + 1][300 + 1];

/**
 * @brief Perform the loop for training Restricted Boltzmann Machine
 */
BENCHMARK_FN(codelet_rbm_train_line191, {
	for (int v = 0; v <= 500; v += 1) {
		for (int h = 0; h <= 300; h += 1) {
			edges[v][h] = edges[v][h] + 0.1 * (pos[v][h] - neg[v][h]);
		}
	}
})

/**
 * @brief Run the benchmarks for codelet_rbm_train_line191
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(1000, oss)
					  .RUN(codelet_rbm_train_line191)
					  .doNotOptimizeAway(edges)
					  .doNotOptimizeAway(pos)
					  .doNotOptimizeAway(neg)
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
