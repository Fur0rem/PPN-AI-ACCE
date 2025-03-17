/**
 * @file include/src/parsing/cycles_encoders.cpp
 * @brief The implementation of the classes to encode and decode the number of cycles.
 */

#include "parsing/cycles_encoders.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
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

std::vector<float> CyclesSplitterEncoder::encode(std::vector<float>& input) {
	if (input.size() != 1) {
		throw std::runtime_error("The input vector must contain a single value");
	}
	if (input[0] < 0) {
		throw std::runtime_error("The number of cycles must be positive");
	}
	if (input[0] > MAX_CYCLES) {
		throw std::runtime_error("The number of cycles must be less than the maximum number of cycles");
	}
	float cycles = input[0];
	float billions = cycles / 1e9F;
	cycles -= billions * 1e9F;
	float millions = cycles / 1e6F;
	cycles -= millions * 1e6F;
	float thousands = cycles / 1e3F;
	cycles -= thousands * 1e3F;
	float units = cycles;
	return {billions / 4.0F, millions / 1000.0F, thousands / 1000.0F, units / 1000.0F};
}

std::vector<float> CyclesSplitterEncoder::decode(std::vector<float>& input) {
	if (input.size() != 4) {
		throw std::runtime_error("The input vector must contain 4 values");
	}
	float billions = input[0] * 4.0F;
	float millions = input[1] * 1000.0F;
	float thousands = input[2] * 1000.0F;
	float units = input[3] * 1000.0F;
	return {billions * 1e9F + millions * 1e6F + thousands * 1e3F + units};
}

CyclesLogEncoder::CyclesLogEncoder(float log_base, float added) : m_log_base(log_base), m_added(added) {}

std::vector<float> CyclesLogEncoder::encode(std::vector<float>& input) {
	if (input.size() != 1) {
		throw std::runtime_error("The input vector must contain a single value");
	}
	if (input[0] < 0) {
		throw std::runtime_error("The number of cycles must be positive");
	}
	if (input[0] > MAX_CYCLES) {
		throw std::runtime_error("The number of cycles must be less than the maximum number of cycles");
	}
	float encoded = m_added + (std::log(1.0F + input[0]) / std::log(m_log_base));
	float max_possible = m_added + (std::log(1.0F + MAX_CYCLES) / std::log(m_log_base));

	assert(encoded <= max_possible);

	return {encoded / max_possible};
}

std::vector<float> CyclesLogEncoder::decode(std::vector<float>& input) {
	if (input.size() != 1) {
		throw std::runtime_error("The input vector must contain a single value");
	}

	float max_possible = m_added + (std::log(MAX_CYCLES) / std::log(m_log_base));
	float denormalised = input[0] * max_possible;
	return {std::pow(m_log_base, denormalised - m_added) - 1.0F};
}

CyclesSqrtEncoder::CyclesSqrtEncoder(float added) : m_added(added) {}

std::vector<float> CyclesSqrtEncoder::encode(std::vector<float>& input) {
	if (input.size() != 1) {
		throw std::runtime_error("The input vector must contain a single value");
	}
	if (input[0] < 0) {
		throw std::runtime_error("The number of cycles must be positive");
	}
	if (input[0] > MAX_CYCLES) {
		throw std::runtime_error("The number of cycles must be less than the maximum number of cycles");
	}

	float encoded = m_added + std::sqrt(input[0]);
	float max_possible = m_added + std::sqrt(MAX_CYCLES);

	assert(encoded <= max_possible);

	return {encoded / max_possible};
}

std::vector<float> CyclesSqrtEncoder::decode(std::vector<float>& input) {
	if (input.size() != 1) {
		throw std::runtime_error("The input vector must contain a single value");
	}

	float max_possible = m_added + std::sqrt(MAX_CYCLES);
	float denormalised = input[0] * max_possible;

	return {(denormalised - m_added) * (denormalised - m_added)};
}

std::vector<float> CyclesBoundedNormaliserEncoder::encode(std::vector<float>& input) {
	float normalised = (input[0] / MAX_CYCLES);
	float bounded = 0.15F + normalised * 0.7F;
	return {bounded};
}

std::vector<float> CyclesBoundedNormaliserEncoder::decode(std::vector<float>& input) {
	if (input[0] < 0.15F) {
		input[0] = 0.15F;
	}
	if (input[0] > 0.85F) {
		input[0] = 0.85F;
	}
	float normalised = (input[0] - 0.15F) / 0.7F;
	return {normalised * MAX_CYCLES};
}
