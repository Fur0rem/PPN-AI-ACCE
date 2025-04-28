/**
 * @file include/parsing/non_encoder.hpp
 * @brief The header for an encoder that does not transform the input
 */

#ifndef NON_ENCODER_HPP
#define NON_ENCODER_HPP

#include "parsing/iencoder.hpp"

/**
 * @brief An encoder that does not transform the input
 */
class NonEncoder : public IEncoder {
  public:
	/**
	 * @brief Does nothing
	 */
	std::vector<float> encode(std::vector<float>& input) const override;

	/**
	 * @brief Does nothing
	 */
	std::vector<float> decode(std::vector<float>& input) const override;

	/**
	 * @brief Does nothing
	 */
	Eigen::MatrixXf encode_batch(const Eigen::MatrixXf& input) const override;

	/**
	 * @brief Does nothing
	 */
	Eigen::MatrixXf decode_batch(const Eigen::MatrixXf& input) const override;
};

#endif // NON_ENCODER_HPP