/**
 * @file non_encoder.cpp
 * @brief Implementation of the NonEncoder class
 */

#include "parsing/non_encoder.hpp"

std::vector<float> NonEncoder::encode(std::vector<float>& input) const {
	return input;
}

std::vector<float> NonEncoder::decode(std::vector<float>& input) const {
	return input;
}

Eigen::MatrixXf NonEncoder::encode_batch(const Eigen::MatrixXf& input) const {
	return input;
}

Eigen::MatrixXf NonEncoder::decode_batch(const Eigen::MatrixXf& input) const {
	return input;
}