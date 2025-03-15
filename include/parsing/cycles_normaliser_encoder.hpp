/**
 * @file include/parsing/cycles_normaliser_encoder.hpp
 * @brief The header for a class to encode and decode the number of cycles by normalising it.
 */

#ifndef CYCLES_ENCODER_HPP
#define CYCLES_ENCODER_HPP

constexpr float MAX_CYCLES = 4e9; ///< Maximum number of cycles as an output for the neural network

#include "parsing/iencoder.hpp"

/**
 * @brief A class to encode and decode the number of cycles.
 */
class CyclesNormaliserEncoder : public IEncoder {
  public:
	/**
	 * @brief Encode the number of cycles by normalising it (dividing by the maximum number of cycles)
	 * @param input The number of cycles
	 * @return The normalised number of cycles, a vector containing a single value between 0 and 1
	 */
	std::vector<float> encode(std::vector<float>& input) override;

	/**
	 * @brief Decode the number of cycles by denormalising it (multiplying by the maximum number of cycles)
	 * @param input The normalised number of cycles
	 * @return The denormalised number of cycles, a vector containing a single value
	 */
	std::vector<float> decode(std::vector<float>& input) override;
};

#endif // CYCLES_ENCODER_HPP