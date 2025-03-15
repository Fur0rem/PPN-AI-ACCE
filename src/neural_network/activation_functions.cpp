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
	return x * (1 - x);
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