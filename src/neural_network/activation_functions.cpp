/**
 * @file: src/neural_network/activation_functions.cpp
 * @brief: Implementation of activation functions for neural networks
 */

#include "neural_network/activation_functions.hpp"
#include <cmath>
#include <eigen3/Eigen/Dense>

float Sigmoid::func(float x) {
	return 1.0F / (1 + std::exp(-x));
}

float Sigmoid::deriv(float x) {
	return Sigmoid::func(x) * (1 - Sigmoid::func(x));
}

float ReLU::func(float x) {
	if (x <= 0) {
		return 0;
	}
	else {
		return x;
	}
}

float ReLU::deriv(float x) {
	if (x <= 0) {
		return 0;
	}
	else {
		return 1;
	}
}

float LeakyReLU::func(float x) {
	if (x <= 0) {
		return 0.01F * x;
	}
	else {
		return x;
	}
}

float LeakyReLU::deriv(float x) {
	if (x <= 0) {
		return 0.01;
	}
	else {
		return 1;
	}
}

float Tanh::func(float x) {
	return std::tanh(x);
}

float Tanh::deriv(float x) {
	return 1 - std::pow(std::tanh(x), 2);
}

float ELU::func(float x) {
	if (x > 0) {
		return x;
	}
	else {
		return 0.01F * (std::exp(x) - 1);
	}
}

float ELU::deriv(float x) {
	if (x > 0) {
		return 1;
	}
	else {
		return 0.01F * std::exp(x);
	}
}

float GELU::func(float x) {
	return 0.5F * x * (1.0F + std::tanh(sqrtf(2.0f / M_PI) * (x + 0.044715F * x * x * x)));
}

float GELU::deriv(float x) {
	float cdf = 0.5F * (1.0F + std::tanh(sqrtf(2.0F / M_PI) * (x + 0.044715F * x * x * x)));
	return 0.5F * (1 + cdf + x * (1 - cdf * cdf) * std::exp(-0.5F * x * x));
}
