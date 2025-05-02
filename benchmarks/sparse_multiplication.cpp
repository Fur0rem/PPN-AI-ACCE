/**
 * @file benchmarks/sparse_multiplication.cpp
 * @brief Benchmark for sparse multiplication of matrices.
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

struct BenchmarkResult {
	double min;
	double max;
	double median;
};

void print_average_results(const std::vector<BenchmarkResult>& results) {
	double min = 0.0;
	double max = 0.0;
	double median = 0.0;
	for (const auto& result : results) {
		min += result.min;
		max += result.max;
		median += result.median;
	}
	min /= results.size();
	max /= results.size();
	median /= results.size();
	std::cout << "Average results: Min: " << min << "s, Max: " << max << "s, Med: " << median << "s\n";
}

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

	// Print the size
	std::cout << "Input matrix size: " << input_matrix.rows() << "x" << input_matrix.cols() << '\n';

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

	// Do it on random batches of the input matrix
	std::vector<BenchmarkResult> no_sparse_results;
	std::vector<BenchmarkResult> sparse_results;
	size_t batch_size = 16;
	for (size_t i = 0; i < 10; i++) {
		// Shuffle the input matrix
		Eigen::MatrixXf shuffled_input_matrix = input_matrix;
		std::vector<size_t> indices(shuffled_input_matrix.rows());
		for (size_t i = 0; i < shuffled_input_matrix.rows(); i++) {
			indices[i] = i;
		}
		std::shuffle(indices.begin(), indices.end(), std::mt19937(std::random_device()()));
		Eigen::MatrixXf x_shuffled(shuffled_input_matrix.rows(), shuffled_input_matrix.cols());
		// #pragma omp parallel for
		for (size_t j = 0; j < shuffled_input_matrix.cols(); j++) {
			for (size_t k = 0; k < shuffled_input_matrix.rows(); k++) {
				x_shuffled(k, j) = shuffled_input_matrix(indices[k], j);
			}
		}

		// Batch
		for (size_t j = 0; j < shuffled_input_matrix.rows(); j += batch_size) {
			size_t end = std::min(j + batch_size, (size_t)shuffled_input_matrix.rows());
			Eigen::MatrixXf x_batch(end - j, shuffled_input_matrix.cols());
			// #pragma omp parallel for
			for (size_t k = 0; k < shuffled_input_matrix.cols(); k++) {
				for (size_t l = j; l < end; l++) {
					x_batch(l - j, k) = x_shuffled(l, k);
				}
			}

			// Benchmark the multiplication
			auto result = ankerl::nanobench::Bench()
							  .epochs(3)
							  .performanceCounters(true)
							  .output(&oss)

							  .run("Eigen Sparse Multiplication",
								   [&]() {
									   Eigen::MatrixXf result = eigen_sparse_multiplication(x_batch, random_matrix);
								   })
							  .run("Eigen Multiplication",
								   [&]() {
									   Eigen::MatrixXf result = x_batch * random_matrix;
								   })
							  .results();
			// Print oss
			// std::cout << oss.str() << '\n';
			for (auto const& res : result) {
				auto measure = res.fromString("elapsed");
				auto name = res.config().mBenchmarkName;
				// std::cout << name << ", Min: " << res.minimum(measure) << "s, Max: " << res.maximum(measure)
				//   << "s, Med: " << res.median(measure) << "s\n";
				if (name == "Eigen Sparse Multiplication") {
					sparse_results.push_back({res.minimum(measure), res.maximum(measure), res.median(measure)});
				}
				else if (name == "Eigen Multiplication") {
					no_sparse_results.push_back({res.minimum(measure), res.maximum(measure), res.median(measure)});
				}

				// Print the sparse results
				std::cout << "Sparse results Median: " << res.median(measure) << "s\n";
			}
			// std::cout << "----------------------------\n";
		}
	}
	std::cout << "Dense results:\n";
	print_average_results(no_sparse_results);
	std::cout << "Sparse results:\n";
	print_average_results(sparse_results);
}