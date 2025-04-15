/**
 * @file include/parsing/iencoder.hpp
 * @brief The header for an interface class for encoding and decoding data.
 */

#ifndef IENCODER_HPP
#define IENCODER_HPP

#include <vector>

/**
 * @brief Interface for encoding and decoding data
 */
class IEncoder {
  public:
	/**
	 * @brief Encode a vector of floats into another vector of floats
	 * @param input The vector of floats to encode
	 * @return The encoded vector of floats
	 */
	virtual std::vector<float> encode(std::vector<float>& input) const = 0;

	/**
	 * @brief Decode a vector of floats into another vector of floats
	 * @param input The vector of floats to decode
	 * @return The decoded vector of floats
	 */
	virtual std::vector<float> decode(std::vector<float>& input) const = 0;
};

#endif // IENCODER_HPP