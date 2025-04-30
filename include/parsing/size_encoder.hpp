/**
 * @file include/parsing/size_encoder.hpp
 * @brief The header for a class to encode and decode a variable size vector.
 */

#ifndef SIZE_ENCODER_HPP
#define SIZE_ENCODER_HPP

#include <cstddef>

constexpr size_t MAX_TOKENS_NN = 11000; ///< Maximum number of tokens for the neural network
constexpr double NO_MORE_VALUE = 0.0;	///< Filler value for the neural network input

#include "parsing/iencoder.hpp"

/**
 * @brief A class to encode and decode a variable size vector.
 */
class SizeEncoder : public IEncoder {
  private:
	size_t m_size; ///< The size of the vector to encode
  public:
	/**
	 * @brief Construct a new SizeEncoder object
	 * @param size The size of the vector to encode
	 */
	SizeEncoder(size_t size);

	/**
	 * @brief Encode a vector by adding a size token at the beginning and a filler value at the end
	 * @param input The vector to encode
	 * @return The encoded vector
	 */
	std::vector<float> encode(std::vector<float>& input) const override;

	/**
	 * @brief Decode a vector by removing the size token at the beginning and the filler value at the end
	 * @param input The vector to decode
	 * @return The decoded vector
	 */
	std::vector<float> decode(std::vector<float>& input) const override;

	/**
	 * @brief Encode a batch of vectors by adding a size token at the beginning and a filler value at the end
	 * @param input The batch of vectors to encode
	 * @return The encoded batch of vectors
	 */
	Eigen::MatrixXf encode_batch(const Eigen::MatrixXf& input) const override;

	/**
	 * @brief Decode a batch of vectors by removing the size token at the beginning and the filler value at the end
	 * @param input The batch of vectors to decode
	 * @return The decoded batch of vectors
	 */
	Eigen::MatrixXf decode_batch(const Eigen::MatrixXf& input) const override;
};

#endif // SIZE_ENCODER_HPP