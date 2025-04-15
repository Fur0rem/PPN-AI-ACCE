/**
 * @file include/parsing/cycles_encoders.hpp
 * @brief The header for a few classes to encode and decode the number of cycles.
 */

#ifndef CYCLES_ENCODERS_HPP
#define CYCLES_ENCODERS_HPP

constexpr float MAX_CYCLES = 4e9; ///< Maximum number of cycles as an output for the neural network

#include "parsing/iencoder.hpp"

/**
 * @brief A class to encode and decode the number of cycles by normalising it (dividing by the maximum number of cycles), to have a value
 * between 0 and 1
 */
class CyclesNormaliserEncoder : public IEncoder {
  public:
	/**
	 * @brief Encode the number of cycles by normalising it (dividing by the maximum number of cycles)
	 * @param input The number of cycles
	 * @return The normalised number of cycles, a vector containing a single value between 0 and 1
	 */
	std::vector<float> encode(std::vector<float>& input) const override;

	/**
	 * @brief Decode the number of cycles by denormalising it (multiplying by the maximum number of cycles)
	 * @param input The normalised number of cycles
	 * @return The denormalised number of cycles, a vector containing a single value
	 */
	std::vector<float> decode(std::vector<float>& input) const override;
};

/**
 * @brief A class to encode and decode the number of cycles by splitting it into 4 values, billions, millions, thousands and units
 */
class CyclesSplitterEncoder : public IEncoder {
  public:
	/**
	 * @brief Encode the number of cycles by splitting it into 4 values, billions, millions, thousands and units
	 * @param input The number of cycles
	 * @return The split number of cycles, a vector containing 4 values
	 */
	std::vector<float> encode(std::vector<float>& input) const override;

	/**
	 * @brief Decode the number of cycles by merging the 4 values into a single one
	 * @param input The split number of cycles
	 * @return The merged number of cycles, a vector containing a single value
	 */
	std::vector<float> decode(std::vector<float>& input) const override;
};

/**
 * @brief A class to encode and decode the number of cycles by normalising it, but between 0.15 and 0.85 to avoid either very low or very
 * high values
 */
class CyclesBoundedNormaliserEncoder : public IEncoder {
  public:
	/**
	 * @brief Encode the number of cycles by normalising it, but between 0.15 and 0.85
	 * @param input The number of cycles
	 * @return The normalised number of cycles, a vector containing a single value between 0.15 and 0.85
	 */
	std::vector<float> encode(std::vector<float>& input) const override;

	/**
	 * @brief Decode the number of cycles by denormalising it, but between 0.15 and 0.85
	 * @param input The normalised number of cycles
	 * @return The denormalised number of cycles, a vector containing a single value
	 */
	std::vector<float> decode(std::vector<float>& input) const override;
};

/**
 * @brief A class to encode and decode the number of cycles by normalising it in a logarithmic scale
 */
class CyclesLogEncoder : public IEncoder {
  private:
	float m_log_base; ///< The base of the logarithm to use
	float m_added;	  ///< A constant to add to the value after the logarithm

  public:
	/**
	 * @brief Construct a new Cycles Log Encoder object
	 * @param[in] log_base The base of the logarithm to use
	 * @param[in] added A constant to add to the value after the logarithm
	 */
	CyclesLogEncoder(float log_base, float added);

	/**
	 * @brief Encode the number of cycles by normalising it in a logarithmic scale
	 * @param input The number of cycles
	 * @return The normalised number of cycles, a vector containing a single value
	 */
	std::vector<float> encode(std::vector<float>& input) const override;

	/**
	 * @brief Decode the number of cycles by denormalising it in a logarithmic scale
	 * @param input The normalised number of cycles
	 * @return The denormalised number of cycles, a vector containing a single value
	 */
	std::vector<float> decode(std::vector<float>& input) const override;
};

/**
 * @brief A class to encode and decode the number of cycles by using the square root of the value
 */
class CyclesSqrtEncoder : public IEncoder {
  private:
	float m_added; ///< A constant to add to the value before the square root
  public:
	/**
	 * @brief Construct a new Cycles Sqrt Encoder object
	 * @param[in] added A constant to add to the value before the square root
	 */
	CyclesSqrtEncoder(float added);

	/**
	 * @brief Encode the number of cycles by using the square root of the value
	 * @param input The number of cycles
	 * @return The normalised number of cycles, a vector containing a single value
	 */
	std::vector<float> encode(std::vector<float>& input) const override;

	/**
	 * @brief Decode the number of cycles by using the square of the value
	 * @param input The normalised number of cycles
	 * @return The denormalised number of cycles, a vector containing a single value
	 */
	std::vector<float> decode(std::vector<float>& input) const override;
};

#endif // CYCLES_ENCODERS_HPP