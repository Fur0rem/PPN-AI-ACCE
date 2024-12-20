#include "neural_network/layer.hpp"
#include "neural_network/activation_functions.hpp"
#include <eigen3/Eigen/Dense>
#include <functional>

Layer::Layer(int input_size, int output_size) {
	m_weights = Eigen::MatrixXf::Random(input_size, output_size);
	m_biases = Eigen::MatrixXf::Random(1, output_size);
}

Eigen::MatrixXf Layer::feed_forward(Eigen::MatrixXf& input) {
	this->m_values = input;
	input = input * this->m_weights + this->m_biases;
	return input.unaryExpr(std::function<float(float)>(sigmoid));
}

Eigen::MatrixXf Layer::back_propagate(const Eigen::MatrixXf& error, float learning_rate, Eigen::MatrixXf& next_values) {
	// Calculate the gradients
	Eigen::MatrixXf prev_errors = error * this->m_weights.transpose();
	Eigen::MatrixXf outputs = next_values.unaryExpr(std::function<float(float)>(deriv_sigmoid));
	Eigen::MatrixXf gradients = error.cwiseProduct(outputs) * learning_rate;
	Eigen::MatrixXf weight_gradients = this->m_values.transpose() * gradients;

	// Adjusting bias and weight
	this->m_biases = this->m_biases + gradients;
	this->m_weights = this->m_weights + weight_gradients;

	return prev_errors;
}