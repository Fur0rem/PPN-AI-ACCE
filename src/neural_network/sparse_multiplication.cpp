/**
 * @file src/neural_network/sparse_multiplication.cpp
 * @brief Implementation of sparse multiplication functions for neural networks
 */

#include "neural_network/sparse_multiplication.hpp"
#include <eigen3/Eigen/src/Core/Matrix.h>

Eigen::MatrixXf naive_multiplication(const Eigen::MatrixXf& sparse_matrix, const Eigen::MatrixXf& dense_matrix) {
	// Ensure the matrices can be multiplied
	assert(sparse_matrix.cols() == dense_matrix.rows() && "Matrix dimensions do not match for multiplication");

	// Dimensions of the matrices
	size_t m_dim = sparse_matrix.rows();
	size_t n_dim = dense_matrix.cols();
	size_t k_dim = sparse_matrix.cols();

	Eigen::MatrixXf result = Eigen::MatrixXf::Zero(m_dim, n_dim);

	for (size_t i = 0; i < m_dim; i++) {
		for (size_t j = 0; j < n_dim; j++) {
			double sum = 0.0;
			for (size_t k = 0; k < k_dim; k++) {
				sum += sparse_matrix(i, k) * dense_matrix(k, j);
			}
			result(i, j) = sum;
		}
	}
	return result;
}

// Matrix sparse is of kind
// X X 0 0 0
// X 0 0 0 0
// X X X 0 0
// where for all lines, there is a point at which there are only zeros left
Eigen::MatrixXf sparse_multiplication(const Eigen::MatrixXf& sparse_matrix, const Eigen::MatrixXf& dense_matrix) {
	// Ensure the matrices can be multiplied
	assert(sparse_matrix.cols() == dense_matrix.rows() && "Matrix dimensions do not match for multiplication");

	// Dimensions of the matrices
	size_t m_dim = sparse_matrix.rows();
	size_t n_dim = dense_matrix.cols();
	size_t k_dim = sparse_matrix.cols();

	Eigen::MatrixXf result = Eigen::MatrixXf::Zero(m_dim, n_dim);

	std::vector<size_t> thresholds(m_dim);
	// The first element of each row contains the size as 1 / size of the row
	for (size_t i = 0; i < m_dim; i++) {
		thresholds[i] = (sparse_matrix.cols() * (float)sparse_matrix(i, 0)) + 1.0F;
	}

	// Cache blocked version with sparse multiplication, in column-major order
	for (size_t i = 0; i < m_dim; i++) {
		for (size_t j = 0; j < n_dim; j++) {
			double sum = 0.0;
			for (size_t k = 0; k < thresholds[i]; k++) {
				sum += sparse_matrix(i, k) * dense_matrix(k, j);
			}
			result(i, j) = sum;
		}
	}
	return result;
}

// Matrix sparse is of kind
// X X 0 0 0
// X 0 0 0 0
// X X X 0 0
// where for all lines, there is a point at which there are only zeros left
Eigen::MatrixXf sparse_multiplication_blocked(const Eigen::MatrixXf& sparse_matrix, const Eigen::MatrixXf& dense_matrix,
											  size_t block_size) {

	auto a = sparse_matrix;
	auto b = dense_matrix;
	Eigen::MatrixXf c = Eigen::MatrixXf::Zero(a.rows(), b.cols());
	// Ensure the matrices can be multiplied
	assert(a.cols() == b.rows() && "Matrix dimensions do not match for multiplication");
	assert(a.rows() == c.rows() && b.cols() == c.cols() && "Result matrix dimensions do not match");

	// Dimensions of the matrices
	size_t m_dim = a.rows(); // Rows of A and C
	size_t n_dim = b.cols(); // Columns of B and C
	size_t k_dim = a.cols(); // Columns of A and rows of B

	Eigen::MatrixXf b_transposed = b.transpose(); // Create a transposed copy of B

	// Compute the thresholds for each row
	std::vector<size_t> thresholds(m_dim);
	for (size_t i = 0; i < m_dim; i++) {
		thresholds[i] = (a.cols() * (float)a(i, 0)) + 1.0F;
	}

	// Cache-blocked GEMM
	for (size_t j_block = 0; j_block < n_dim; j_block += block_size) {
		for (size_t k_block = 0; k_block < k_dim; k_block += block_size) {
			for (size_t i_block = 0; i_block < m_dim; i_block += block_size) {
				size_t i_end = std::min(i_block + block_size, m_dim);
				size_t j_end = std::min(j_block + block_size, n_dim);
				size_t k_end = std::min(k_block + block_size, k_dim);

				for (size_t j = j_block; j < j_end; ++j) {
					for (size_t i = i_block; i < i_end; ++i) {
						float sum = 0.0f;
						for (size_t k = k_block; k < std::min(k_end, thresholds[i]); ++k) {
							sum += a(i, k) * b_transposed(j, k);
						}
						c(i, j) += sum;
					}
				}
			}
		}
	}
	// Return the result
	return c;
}

#include <eigen3/Eigen/Sparse>

Eigen::MatrixXf eigen_sparse_multiplication(const Eigen::MatrixXf& sparse_matrix, const Eigen::MatrixXf& dense_matrix) {
	// Ensure the matrices can be multiplied
	assert(sparse_matrix.cols() == dense_matrix.rows() && "Matrix dimensions do not match for multiplication");

	// Dimensions of the matrices
	size_t m_dim = sparse_matrix.rows();
	size_t n_dim = dense_matrix.cols();
	size_t k_dim = sparse_matrix.cols();

	Eigen::MatrixXf result = Eigen::MatrixXf::Zero(m_dim, n_dim);

	// Compute the thresholds for each row
	// std::vector<size_t> thresholds(m_dim);
	size_t threshold = 0;
	for (size_t i = 0; i < m_dim; i++) {
		// thresholds[i] = (sparse_matrix.cols() * (float)sparse_matrix(i, 0)) + 1.0F;
		threshold = std::max(threshold, (size_t)((sparse_matrix.cols() * (float)sparse_matrix(i, 0)) + 1.0F));
	}

	// Eigen's block feature to only the multiplication of the non-zero elements
	result.block(0, 0, m_dim, n_dim) = sparse_matrix.block(0, 0, m_dim, threshold) * dense_matrix.block(0, 0, threshold, n_dim);

	return result;
}