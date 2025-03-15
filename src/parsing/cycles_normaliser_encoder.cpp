/**
 * @file include/src/cycles_normaliser_encoder.hpp
 * @brief The header for a class to encode and decode the number of cycles by normalising it.
 */

#include "parsing/cycles_normaliser_encoder.hpp"
#include <stdexcept>
#include <vector>

std::vector<float> CyclesNormaliserEncoder::encode(std::vector<float>& input) {
	if (input.size() != 1) {
		throw std::runtime_error("The input vector must contain a single value");
	}
	if (input[0] < 0) {
		throw std::runtime_error("The number of cycles must be positive");
	}
	if (input[0] > MAX_CYCLES) {
		throw std::runtime_error("The number of cycles must be less than the maximum number of cycles");
	}
	return {input[0] / MAX_CYCLES};
}

std::vector<float> CyclesNormaliserEncoder::decode(std::vector<float>& input) {
	if (input.size() != 1) {
		throw std::runtime_error("The input vector must contain a single value");
	}
	return {input[0] * MAX_CYCLES};
}