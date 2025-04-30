/**
 * @file include/neural_network/sparse_multiplication.hpp
 * @brief Header file for sparse matrix multiplication functions
 */

#include <eigen3/Eigen/Dense>

/**
 * @brief Naive matrix multiplication
 * @details This function multiplies a sparse matrix with a dense matrix without any optimisation on neither the non-zero values nor the
 * cache.
 * @param sparse_matrix The sparse matrix
 * @param dense_matrix The dense matrix
 */
Eigen::MatrixXf naive_multiplication(const Eigen::MatrixXf& sparse_matrix, const Eigen::MatrixXf& dense_matrix);

/**
 * @brief Sparse matrix multiplication
 * @details This function multiplies a sparse matrix with a dense matrix.
 * The sparse matrix has, for each row, all of its non-zero values at the beginning of the row and the rest are zeros.
 * The number of zeroes is stored in the first element of each row.
 * Example :
 * 2/5 X X 0 0 0
 * 1/5 X 0 0 0 0
 * 3/5 X X X 0 0
 * @param sparse_matrix The sparse matrix
 * @param dense_matrix The dense matrix
 * @return The result of the multiplication
 */
Eigen::MatrixXf sparse_multiplication(const Eigen::MatrixXf& sparse_matrix, const Eigen::MatrixXf& dense_matrix);

/**
 * @brief Cache-blocked sparse matrix multiplication.
 * Similar to sparse_multiplication but uses cache-blocking for better performance.
 * @see sparse_multiplication
 * @param sparse_matrix The sparse matrix
 * @param dense_matrix The dense matrix
 * @param block_size The size of the blocks to use for the multiplication
 * @return The result of the multiplication
 */
Eigen::MatrixXf sparse_multiplication_blocked(const Eigen::MatrixXf& sparse_matrix, const Eigen::MatrixXf& dense_matrix, size_t block_size);

/**
 * @brief Eigen sparse matrix multiplication
 * @details This function multiplies a sparse matrix with a dense matrix using Eigen's built-in functions.
 * @param sparse_matrix The sparse matrix
 * @param dense_matrix The dense matrix
 * @return The result of the multiplication
 */
Eigen::MatrixXf eigen_sparse_multiplication(const Eigen::MatrixXf& sparse_matrix, const Eigen::MatrixXf& dense_matrix);
