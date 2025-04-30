/**
 * @file codelets/codelet_c_k_means_line109.cpp
 * @brief Benchmark for k-means clustering
 */

#include "bench_common.hpp"

constexpr int K = 16;
constexpr int M = 16;
double c[K][M];
int counts[K];
double c1[K][M];

/**
 * @brief Perform the k-means clustering
 */
BENCHMARK_FN(codelet_c_k_means_line109, {
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < M; j++) {
			c[i][j] = (counts[i] ? c1[i][j] / counts[i] : c1[i][j]);
		}
	}
})

/**
 * @brief Run the benchmarks for codelet_c_k_means_line109
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(1000, oss).RUN(codelet_c_k_means_line109).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
