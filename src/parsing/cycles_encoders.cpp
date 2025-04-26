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

// CyclesNormaliserEncoder implementations

std::vector<float> CyclesNormaliserEncoder::encode(std::vector<float>& input) const {
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

std::vector<float> CyclesNormaliserEncoder::decode(std::vector<float>& input) const {
	if (input.size() != 1) {
		throw std::runtime_error("The input vector must contain a single value");
	}
	return {input[0] * MAX_CYCLES};
}

Eigen::MatrixXf CyclesNormaliserEncoder::encode_batch(const Eigen::MatrixXf& input) const {
	Eigen::MatrixXf result(input.rows(), 1);
	for (size_t i = 0; i < input.rows(); i++) {
		result(i, 0) = input(i, 0) / MAX_CYCLES;
	}
	return result;
}

Eigen::MatrixXf CyclesNormaliserEncoder::decode_batch(const Eigen::MatrixXf& input) const {
	Eigen::MatrixXf result(input.rows(), 1);
	for (size_t i = 0; i < input.rows(); i++) {
		result(i, 0) = input(i, 0) * MAX_CYCLES;
	}
	return result;
}

// CyclesSplitterEncoder implementations

std::vector<float> CyclesSplitterEncoder::encode(std::vector<float>& input) const {
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

std::vector<float> CyclesSplitterEncoder::decode(std::vector<float>& input) const {
	if (input.size() != 4) {
		throw std::runtime_error("The input vector must contain 4 values");
	}
	float billions = input[0] * 4.0F;
	float millions = input[1] * 1000.0F;
	float thousands = input[2] * 1000.0F;
	float units = input[3] * 1000.0F;
	return {billions * 1e9F + millions * 1e6F + thousands * 1e3F + units};
}

Eigen::MatrixXf CyclesSplitterEncoder::encode_batch(const Eigen::MatrixXf& input) const {
	Eigen::MatrixXf result(input.rows(), 4);
	for (size_t i = 0; i < input.rows(); i++) {
		result(i, 0) = (input(i, 0) / 1e9F) / 4.0F;
		result(i, 1) = (input(i, 0) / 1e6F) / 1000.0F;
		result(i, 2) = (input(i, 0) / 1e3F) / 1000.0F;
		result(i, 3) = input(i, 0) / 1000.0F;
	}
	return result;
}

Eigen::MatrixXf CyclesSplitterEncoder::decode_batch(const Eigen::MatrixXf& input) const {
	Eigen::MatrixXf result(input.rows(), 1);
	for (size_t i = 0; i < input.rows(); i++) {
		float billions = input(i, 0) * 4.0F;
		float millions = input(i, 1) * 1000.0F;
		float thousands = input(i, 2) * 1000.0F;
		float units = input(i, 3) * 1000.0F;
		result(i, 0) = billions * 1e9F + millions * 1e6F + thousands * 1e3F + units;
	}
	return result;
}

// CyclesLogEncoder implementations

CyclesLogEncoder::CyclesLogEncoder(float log_base, float added) : m_log_base(log_base), m_added(added) {}

std::vector<float> CyclesLogEncoder::encode(std::vector<float>& input) const {
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

std::vector<float> CyclesLogEncoder::decode(std::vector<float>& input) const {
	if (input.size() != 1) {
		throw std::runtime_error("The input vector must contain a single value");
	}

	float max_possible = m_added + (std::log(MAX_CYCLES) / std::log(m_log_base));
	float denormalised = input[0] * max_possible;
	return {std::pow(m_log_base, denormalised - m_added) - 1.0F};
}

Eigen::MatrixXf CyclesLogEncoder::encode_batch(const Eigen::MatrixXf& input) const {
	Eigen::MatrixXf result(input.rows(), 1);
	for (size_t i = 0; i < input.rows(); i++) {
		// std::cout << "Encoding: " << input(i, 0) << '\n';
		result(i, 0) = m_added + (std::log(1.0F + input(i, 0)) / std::log(m_log_base));
		// std::cout << "Encoded: " << result(i, 0) << '\n';
	}
	return result;
}

Eigen::MatrixXf CyclesLogEncoder::decode_batch(const Eigen::MatrixXf& input) const {
	float max_possible = m_added + (std::log(MAX_CYCLES) / std::log(m_log_base));
	Eigen::MatrixXf result(input.rows(), 1);
	for (size_t i = 0; i < input.rows(); i++) {
		// std::cout << "Decoding: " << input(i, 0) << '\n';
		float denormalised = input(i, 0) * max_possible;
		result(i, 0) = std::pow(m_log_base, denormalised - m_added) - 1.0F;
		// std::cout << "Decoded: " << result(i, 0) << '\n';
	}
	return result;
}

// CyclesSqrtEncoder implementations

CyclesSqrtEncoder::CyclesSqrtEncoder(float added) : m_added(added) {}

std::vector<float> CyclesSqrtEncoder::encode(std::vector<float>& input) const {
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

std::vector<float> CyclesSqrtEncoder::decode(std::vector<float>& input) const {
	if (input.size() != 1) {
		throw std::runtime_error("The input vector must contain a single value");
	}

	float max_possible = m_added + std::sqrt(MAX_CYCLES);
	float denormalised = input[0] * max_possible;

	return {(denormalised - m_added) * (denormalised - m_added)};
}

Eigen::MatrixXf CyclesSqrtEncoder::encode_batch(const Eigen::MatrixXf& input) const {
	Eigen::MatrixXf result(input.rows(), 1);
	for (size_t i = 0; i < input.rows(); i++) {
		result(i, 0) = m_added + std::sqrt(input(i, 0));
	}
	return result;
}

Eigen::MatrixXf CyclesSqrtEncoder::decode_batch(const Eigen::MatrixXf& input) const {
	Eigen::MatrixXf result(input.rows(), 1);
	for (size_t i = 0; i < input.rows(); i++) {
		result(i, 0) = (input(i, 0) - m_added) * (input(i, 0) - m_added);
	}
	return result;
}

// CyclesBoundedNormaliserEncoder implementations

std::vector<float> CyclesBoundedNormaliserEncoder::encode(std::vector<float>& input) const {
	float normalised = (input[0] / MAX_CYCLES);
	float bounded = 0.15F + normalised * 0.7F;
	return {bounded};
}

std::vector<float> CyclesBoundedNormaliserEncoder::decode(std::vector<float>& input) const {
	if (input[0] < 0.15F) {
		input[0] = 0.15F;
	}
	if (input[0] > 0.85F) {
		input[0] = 0.85F;
	}
	float normalised = (input[0] - 0.15F) / 0.7F;
	return {normalised * MAX_CYCLES};
}

Eigen::MatrixXf CyclesBoundedNormaliserEncoder::encode_batch(const Eigen::MatrixXf& input) const {
	Eigen::MatrixXf result(input.rows(), 1);
	for (size_t i = 0; i < input.rows(); i++) {
		result(i, 0) = (input(i, 0) / MAX_CYCLES);
	}
	return result;
}

Eigen::MatrixXf CyclesBoundedNormaliserEncoder::decode_batch(const Eigen::MatrixXf& input) const {
	Eigen::MatrixXf result(input.rows(), 1);
	for (size_t i = 0; i < input.rows(); i++) {
		result(i, 0) = input(i, 0);
		if (input(i, 0) < 0.15F) {
			result(i, 0) = 0.15F;
		}
		if (input(i, 0) > 0.85F) {
			result(i, 0) = 0.85F;
		}
		result(i, 0) = (result(i, 0) - 0.15F) / 0.7F;
	}
	return result;
}
