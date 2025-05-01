/**
 * @file: src/neural_network/activation_functions.cpp
 * @brief: Implementation of activation functions for neural networks
 */

#include "neural_network/activation_functions.hpp"
#include <cmath>
#include <eigen3/Eigen/Dense>

// Taken from https://gist.github.com/jrade/293a73f89dfef51da6522428c857802d
// Paper : https://nic.schraudolph.org/pubs/Schraudolph99.pdf
inline float schraudolph_exp(float x) {
	constexpr float a = (1 << 23) / 0.69314718f;
	constexpr float b = (1 << 23) * (127 - 0.043677448f);
	x = a * x + b;

	// Bounds checking
	constexpr float c = (1 << 23);
	constexpr float d = (1 << 23) * 255;
	if (x < c || x > d) {
		x = (x < c) ? 0.0f : d;
	}

	// Cast it as int
	uint32_t n = static_cast<uint32_t>(x);
	// Transmute the bits to float
	union {
		int i;
		float f;
	} transmute_device;
	transmute_device.i = n;
	x = transmute_device.f;

	return x;
}

float Sigmoid::func(float x) {
	return 1.0F / (1 + schraudolph_exp(-x));
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
		return 0.01F * (schraudolph_exp(x) - 1);
	}
}

float ELU::deriv(float x) {
	if (x > 0) {
		return 1;
	}
	else {
		return 0.01F * schraudolph_exp(x);
	}
}

float GELU::func(float x) {
	return 0.5F * x * (1.0F + std::tanh(sqrtf(2.0f / M_PI) * (x + 0.044715F * x * x * x)));
}

float GELU::deriv(float x) {
	float cdf = 0.5F * (1.0F + std::tanh(sqrtf(2.0F / M_PI) * (x + 0.044715F * x * x * x)));
	return 0.5F * (1 + cdf + x * (1 - cdf * cdf) * schraudolph_exp(-0.5F * x * x));
}
