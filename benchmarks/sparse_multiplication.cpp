/**
 * @file benchmarks/sparse_multiplication.cpp
 * @brief Benchmark for sparse multiplication of matrices.
 */

/**
 * @file benchmarks/training.cpp
 * @brief Benchmark for training the neural network.
 */

#include "neural_network/sparse_multiplication.hpp"
#include "dataset/dataset.hpp"
#include "parsing/cycles_encoders.hpp"
#include "parsing/hexadecimal_parser.hpp"
#include "parsing/size_encoder.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#define ANKERL_NANOBENCH_IMPLEMENT ///< Needed to use nanobench
#include "../nanobench/src/include/nanobench.h"

int main() {
	// Topology of the neural network
	std::vector<size_t> topology = {
		MAX_TOKENS_NN / 8,
		988,
		236,
		1047,
		70,
		1,
	};

	Dataset dataset = Dataset(new HexadecimalParser(),
							  "dataset/bench_bins",
							  topology,
							  std::make_unique<SizeEncoder>(topology[0]),
							  std::make_unique<CyclesLogEncoder>(2, 0));

	auto input_matrix = dataset.get_input_data();

	// Generate a random matrix of size of input matrix to be able to multiply it with it
	Eigen::MatrixXf random_matrix(input_matrix.cols(), input_matrix.cols());
	random_matrix.setRandom();

	std::ostringstream oss;
	auto result = ankerl::nanobench::Bench()
					  .epochs(3)
					  .performanceCounters(true)
					  .output(&oss)
					  .run("Naive Multiplication",
						   [&]() {
							   Eigen::MatrixXf result = naive_multiplication(input_matrix, random_matrix);
						   })
					  .run("Sparse Multiplication",
						   [&]() {
							   Eigen::MatrixXf result = sparse_multiplication(input_matrix, random_matrix);
						   })
					  .run("Blocked-8 Sparse Multiplication",
						   [&]() {
							   Eigen::MatrixXf result = sparse_multiplication_blocked(input_matrix, random_matrix, 8);
						   })
					  .run("Blocked-16 Sparse Multiplication",
						   [&]() {
							   Eigen::MatrixXf result = sparse_multiplication_blocked(input_matrix, random_matrix, 16);
						   })
					  .run("Blocked-32 Sparse Multiplication",
						   [&]() {
							   Eigen::MatrixXf result = sparse_multiplication_blocked(input_matrix, random_matrix, 32);
						   })

					  .run("Eigen Sparse Multiplication",
						   [&]() {
							   Eigen::MatrixXf result = eigen_sparse_multiplication(input_matrix, random_matrix);
						   })
					  .run("Eigen Multiplication",
						   [&]() {
							   Eigen::MatrixXf result = input_matrix * random_matrix;
						   })
					  .results();
	// Print oss
	// std::cout << oss.str() << '\n';

	for (auto const& res : result) {
		auto measure = res.fromString("elapsed");
		auto name = res.config().mBenchmarkName;
		std::cout << name << ", Min: " << res.minimum(measure) << "s, Max: " << res.maximum(measure) << "s, Med: " << res.median(measure)
				  << "s\n";
	}
	std::cout << "----------------------------\n";
}