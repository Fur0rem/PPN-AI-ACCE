/**
 * @file src/parsing/size_encoder.cpp
 * @brief Implementation of the SizeEncoder class
 */

#include "parsing/size_encoder.hpp"
#include <stdexcept>

constexpr const char* RED = "\033[0;31m";
constexpr const char* RESET = "\033[0m";

SizeEncoder::SizeEncoder(size_t size) : m_size(size) {}

std::vector<float> SizeEncoder::encode(std::vector<float>& input) {
	// Check if the input can fit in the neural network
	if (input.size() + 1 > m_size) {
		std::string err_msg = RED + std::string("Warning : Kernel too big to analyse : ") + std::to_string(input.size()) + " > " +
							  std::to_string(m_size - 1) + RESET;
		throw std::runtime_error(err_msg);
	}

	// Create the encoded vector (empty for now)
	std::vector<float> result(m_size, NO_MORE_VALUE);

	// Store the number of tokens in the first element
	if (input.size() > 0) {
		result[0] = static_cast<float>(input.size()) / static_cast<float>(m_size - 1);
	}
	else {
		result[0] = 0.0F;
	}

	// Copy the input into the encoded vector
	for (size_t i = 0; i < input.size(); i++) {
		result[i + 1] = input[i];
	}
	return result;
}

std::vector<float> SizeEncoder::decode(std::vector<float>& input) {
	std::vector<float> result;

	for (size_t i = 1; i < input.size(); i++) {
		if (input[i] == NO_MORE_VALUE) {
			break;
		}
		result.push_back(input[i]);
	}
	return result;
}