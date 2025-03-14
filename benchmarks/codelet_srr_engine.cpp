/**
 * @file benchmarks/codelet_srr_engine.cpp
 * @brief Benchmark of codelet for srr engine
 */

#include "bench_common.hpp"

constexpr int N = 16;		///< Size of the matrices
double m_result[N] = {3.0}; ///< Array of double precision floating point numbers to store the result
double mat1[N][N] = {4.0};	///< First matrix
double mat2[N] = {5.0};		///< Second matrix

/**
 * @brief Perform the loop for matrix multiplication
 */
BENCHMARK_FN(codelet_srr_engine_matmul_line21_1, {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			m_result[i] += mat1[i][j] * mat2[j];
		}
	}
})

/**
 * @brief Perform the loop for matrix multiplication with unrolling
 */
BENCHMARK_FN(codelet_srr_engine_matmul_line21_8, {
	for (int i = 0; i < N; i += 4) {
		for (int j = 0; j < N; j++) {
			m_result[i] += mat1[i][j] * mat2[j];
			m_result[i + 1] += mat1[i + 1][j] * mat2[j];
			m_result[i + 2] += mat1[i + 2][j] * mat2[j];
			m_result[i + 3] += mat1[i + 3][j] * mat2[j];
		}
	}
})

/**
 * @brief Run the benchmarks for codelet_rbm_train_line191
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(1000, oss)
					  .RUN(codelet_srr_engine_matmul_line21_1)
					  .RUN(codelet_srr_engine_matmul_line21_8)
					  .doNotOptimizeAway(m_result)
					  .doNotOptimizeAway(mat1)
					  .doNotOptimizeAway(mat2)
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
