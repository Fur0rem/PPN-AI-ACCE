/**
 * @file tests/sparse_multiplication.cpp
 * @brief Used to test if the sparse multiplication function works for our kind of matrices
 */

#include <gtest/gtest.h>

#include "dataset/dataset.hpp"
#include "neural_network/sparse_multiplication.hpp"
#include "parsing/cycles_encoders.hpp"
#include "parsing/hexadecimal_parser.hpp"
#include "parsing/size_encoder.hpp"

/**
 * @brief Check if the sparse multiplication function works for our kind of matrices
 */
TEST(SparseMultiplication, SparseMultiplication) {
	std::vector<size_t> topology = {
		MAX_TOKENS_NN / 8,
		MAX_TOKENS_NN / 8,
		MAX_TOKENS_NN / 32,
		1,
	};
	Dataset dataset(new HexadecimalParser(),
					"dataset/bench_bins_small",
					topology,
					std::make_unique<SizeEncoder>(MAX_TOKENS_NN / 8),
					std::make_unique<CyclesLogEncoder>(2, 0));
	auto input_matrix = dataset.get_input_data();

	std::cout << "Input matrix size: " << input_matrix.rows() << " x " << input_matrix.cols() << '\n';

	// Generate a random matrix of size of input matrix to be able to multiply it with it
	Eigen::MatrixXf random_matrix(input_matrix.cols(), input_matrix.cols());
	random_matrix.setRandom();

	std::cout << "Random matrix size: " << random_matrix.rows() << " x " << random_matrix.cols() << '\n';

	// Do the sparse multiplication
	Eigen::MatrixXf sparse_result = sparse_multiplication(input_matrix, random_matrix);
	Eigen::MatrixXf sparse_result_blocked = sparse_multiplication_blocked(input_matrix, random_matrix, 16);

	// Check if the result is the same as the normal multiplication
	Eigen::MatrixXf normal_result = input_matrix * random_matrix;

	for (size_t i = 0; i < sparse_result.rows(); i++) {
		for (size_t j = 0; j < sparse_result.cols(); j++) {
			EXPECT_EQ(std::abs(sparse_result(i, j) - normal_result(i, j)) < 1e-4, true)
				<< "Sparse result: " << sparse_result(i, j) << ", Normal result: " << normal_result(i, j) << ", i: " << i << ", j: " << j;
			EXPECT_EQ(std::abs(sparse_result_blocked(i, j) - normal_result(i, j)) < 1e-4, true)
				<< "Sparse blocked result: " << sparse_result_blocked(i, j) << ", Normal result: " << normal_result(i, j) << ", i: " << i
				<< ", j: " << j;
		}
	}
}
