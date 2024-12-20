#include "neural_network/neural_network.hpp"
#include "neural_network/activation_functions.hpp"
#include <cstdlib>
#include <eigen3/Eigen/Dense>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

float mean_squared_error(std::vector<float> output, std::vector<float> targets) {
	float loss = 0;
	for (size_t i = 0; i < targets.size(); i++) {
		loss += (output[i] - targets[i]) * (output[i] - targets[i]);
	}
	return loss / static_cast<float>(targets.size());
}

float mean_absolute_error(std::vector<float> output, std::vector<float> targets) {
	float loss = 0;
	for (size_t i = 0; i < targets.size(); i++) {
		loss += std::abs(output[i] - targets[i]);
	}
	return loss / static_cast<float>(targets.size());
}

NeuralNetwork::NeuralNetwork(std::vector<int>& topology, float learning_rate) : m_topology(topology), m_learning_rate(learning_rate) {
	for (size_t i = 0; i < topology.size() - 1; i++) {
		Layer layer(topology[i], topology[i + 1]);
		m_layers.push_back(layer);
	}
	this->m_last_values = Eigen::MatrixXf::Zero(0, 0);
}

void NeuralNetwork::feed_forward(const Eigen::VectorXf& input) {
	if (input.size() != m_topology[0]) {
		throw std::invalid_argument("Input size does not match the number of weights.");
	}

	Eigen::MatrixXf values2(1, input.size());
	for (long i = 0; i < input.size(); i++) {
		values2(0, i) = input[i];
	}

	// Forwarding inputs to next layers
	Eigen::MatrixXf layer_values = values2;
	for (Layer& layer : m_layers) {
		layer_values = layer.feed_forward(layer_values);
	}

	// Save the last values
	this->m_last_values = layer_values;
}

void NeuralNetwork::back_propagate(const Eigen::VectorXf& target_output) {
	Eigen::MatrixXf errors(1, target_output.size());
	for (long i = 0; i < target_output.size(); i++) {
		errors(0, i) = target_output[i] - m_last_values(0, i);
	}

	// Propagate for the last values of the layer
	errors = m_layers[m_layers.size() - 1].back_propagate(errors, m_learning_rate, m_last_values);

	// Propagate for the rest of the layers
	for (int i = m_layers.size() - 2; i >= 0; i--) {
		errors = m_layers[i].back_propagate(errors, m_learning_rate, m_layers[i + 1].m_values);
	}
}

std::vector<float> NeuralNetwork::get_prediction(std::vector<float>& input) {
	// Convert the input to Eigen::VectorXf
	Eigen::VectorXf input_vector(input.size());
	for (size_t i = 0; i < input.size(); i++) {
		input_vector[i] = input[i];
	}

	// Get the prediction
	feed_forward(input_vector);
	Eigen::VectorXf output = m_last_values;

	// Convert the output to std::vector<float>
	std::vector<float> output_vector(output.size());
	for (long i = 0; i < output.size(); i++) {
		output_vector[i] = output[i];
	}

	return output_vector;
}

float NeuralNetwork::get_total_loss(std::vector<std::vector<float>> inputs, std::vector<std::vector<float>> targets) {
	float loss = 0;
	for (size_t i = 0; i < inputs.size(); i++) {
		loss += mean_squared_error(this->get_prediction(inputs[i]), targets[i]);
	}
	return loss / static_cast<float>(inputs.size());
}

void NeuralNetwork::train(std::vector<std::vector<float>>& inputs, std::vector<std::vector<float>>& targets, int nb_epochs,
						  std::string&& logging_filename) {
	// Open the file for logging
	std::ofstream log_file;
	log_file.open(logging_filename);

	// Convert the inputs and targets to Eigen::VectorXf
	std::vector<Eigen::VectorXf> input_vectors(inputs.size());
	std::vector<Eigen::VectorXf> target_vectors(targets.size());
	for (size_t i = 0; i < inputs.size(); i++) {
		Eigen::VectorXf input(inputs[i].size());
		for (size_t j = 0; j < inputs[i].size(); j++) {
			input[j] = inputs[i][j];
		}
		input_vectors[i] = input;
	}
	for (size_t i = 0; i < targets.size(); i++) {
		Eigen::VectorXf target(targets[i].size());
		for (size_t j = 0; j < targets[i].size(); j++) {
			target[j] = targets[i][j];
		}
		target_vectors[i] = target;
	}

	// Training the neural network
	const int nb_points_to_plot = nb_epochs / 1000;
	for (int i = 0; i < nb_epochs; i++) {
		for (size_t j = 0; j < inputs.size(); j++) {
			this->feed_forward(input_vectors[j]);
			this->back_propagate(target_vectors[j]);
		}
		// Logging the loss
		if (i % nb_points_to_plot == 0) {
			log_file << "Epoch: " << i << ", Loss: " << this->get_total_loss(inputs, targets) << '\n';
		}
	}
}