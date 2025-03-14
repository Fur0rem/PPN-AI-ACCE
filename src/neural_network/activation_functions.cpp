/**
 * @file: src/neural_network/activation_functions.cpp
 * @brief: Implementation of activation functions for neural networks
 */

#include "neural_network/activation_functions.hpp"
#include <cmath>
#include <eigen3/Eigen/Dense>

float sigmoid(float x) {
	return 1.0F / (1 + std::exp(-x));
}

float deriv_sigmoid(float x) {
	return x * (1 - x);
}

float relu(float x) {
	if (x <= 0) {
		return 0;
	}
	else {
		return x;
	}
}

float deriv_relu(float x) {
	if (x <= 0) {
		return 0;
	}
	else {
		return 1;
	}
}

float leaky_relu(float x) {
	if (x <= 0) {
		return 0.05 * x;
	}
	else {
		return x;
	}
}

float deriv_leaky_relu(float x) {
	if (x <= 0) {
		return 0.05;
	}
	else {
		return 1;
	}
}

float deriv_tanh(float x) {
	return 1 - x * x;
}