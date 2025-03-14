/**
 * @file src/neural_network/layer.cpp
 * @brief Implementation of the Layer class
 */

#include "neural_network/layer.hpp"
#include "neural_network/activation_functions.hpp"
#include <eigen3/Eigen/Dense>
#include <functional>

Layer::Layer(int input_size, int output_size) {
	m_weights = Eigen::MatrixXf::Random(input_size, output_size);
	m_biases = Eigen::MatrixXf::Random(1, output_size);
	// Make sure they don't start at 0
	for (int i = 0; i < m_weights.rows(); i++) {
		for (int j = 0; j < m_weights.cols(); j++) {
			if (std::abs(m_weights(i, j)) < 0.05) {
				m_weights(i, j) = 0.05;
			}
		}
	}
	for (int i = 0; i < m_biases.cols(); i++) {
		if (std::abs(m_biases(0, i)) < 0.05) {
			m_biases(0, i) = 0.05;
		}
	}
}

const auto ACTIVATION = sigmoid;
const auto DERIV_ACTIVATION = deriv_sigmoid;

Eigen::MatrixXf Layer::feed_forward(Eigen::MatrixXf* input) {
	this->m_values = *input;
	*input = *input * this->m_weights + this->m_biases;
	return input->unaryExpr(std::function<float(float)>(ACTIVATION));
}

Eigen::MatrixXf Layer::back_propagate(const Eigen::MatrixXf& error, float learning_rate, Eigen::MatrixXf& next_values) {
	// Calculate the gradients
	Eigen::MatrixXf prev_errors = error * this->m_weights.transpose();
	Eigen::MatrixXf outputs = next_values.unaryExpr(std::function<float(float)>(DERIV_ACTIVATION));
	Eigen::MatrixXf gradients = error.cwiseProduct(outputs) * learning_rate;
	Eigen::MatrixXf weight_gradients = this->m_values.transpose() * gradients;

	// Adjusting bias and weight
	this->m_biases = this->m_biases + gradients;
	this->m_weights = this->m_weights + weight_gradients;

	return prev_errors;
}