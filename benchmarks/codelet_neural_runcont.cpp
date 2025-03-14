/**
 * @file benchmarks/dummy.cpp
 * @brief Dummy benchmark as a template for new benchmarks
 */

#include "bench_common.hpp"

constexpr int NN_TOT = 3500; ///< Number of neurons total
volatile float tmp_vec_a[NN_TOT];
volatile signed int tmp_vec_c[NN_TOT];

BENCHMARK_FN(codelet_neural_c_runcont_line711_0, {
	for (int i = 0; i < NN_TOT; ++i) {
		tmp_vec_c[i] = (tmp_vec_a[i] > 0 ? 1 : -1);
	}
})

float dest[NN_TOT];
BENCHMARK_FN(codelet_neural_c_runcont_line720_0, {
	for (int neuron = 0; neuron <= NN_TOT - 1; neuron += 1) {
		tmp_vec_a[neuron] = dest[neuron];
	}
})

void init() {
	for (int i = 0; i < NN_TOT; ++i) {
		dest[i] = 1.0f;
		tmp_vec_a[i] = 1.0f;
		tmp_vec_c[i] = 0;
	}
}

/**
 * @brief Run the benchmarks for dummy
 */
int main() {
	init();
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(1000, oss)
					  .RUN(codelet_neural_c_runcont_line711_0)
					  .RUN(codelet_neural_c_runcont_line720_0)
					  .doNotOptimizeAway(tmp_vec_c)
					  .doNotOptimizeAway(tmp_vec_a)
					  .doNotOptimizeAway(dest)
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
