/**
 * @file src/neural_network/neural_network.cpp
 * @brief Implementation of the NeuralNetwork class
 */

#include "neural_network/neural_network.hpp"
#include "neural_network/activation_functions.hpp"
#include "parsing/iparser.hpp"
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/src/Core/Matrix.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<float> cpp_vec_from_eigen_vec(const Eigen::VectorXf& eigen_vector) {
	std::vector<float> cpp_vector(eigen_vector.size());
	for (long i = 0; i < eigen_vector.size(); i++) {
		cpp_vector[i] = eigen_vector[i];
	}
	return cpp_vector;
}

Eigen::VectorXf eigen_vec_from_cpp_vec(const std::vector<float>& cpp_vector) {
	Eigen::VectorXf eigen_vector(cpp_vector.size());
	for (size_t i = 0; i < cpp_vector.size(); i++) {
		eigen_vector[i] = cpp_vector[i];
	}
	return eigen_vector;
}

/**
 * @brief Calculate the  squared error
 * @param output The output of the network
 * @param targets The target output of the network
 * @return The  squared error
 */
double squared_error(const Eigen::VectorXf* output, const Eigen::VectorXf* targets) {
	assert(targets->size() == 1); // We only have one output (the number of cycles)
	double target = (*targets)[0] * MAX_CYCLES;
	double prediction = (*output)[0] * MAX_CYCLES;
	return (target - prediction) * (target - prediction);
}

/**
 * @brief Calculate the relative squared error
 * @param output The output of the network
 * @param targets The target output of the network
 * @return The squared error
 */
double relative_squared_error(const Eigen::VectorXf* output, const Eigen::VectorXf* targets) {
	assert(targets->size() == 1); // We only have one output (the number of cycles)
	double target = (*targets)[0] * MAX_CYCLES;
	double prediction = (*output)[0] * MAX_CYCLES;
	return (target - prediction) * (target - prediction) / std::max(1.0, std::max(target * target, prediction * prediction));
}

/**
 * @brief Calculate the  absolute error
 * @param output The output of the network
 * @param targets The target output of the network
 * @return The  absolute error
 */
double absolute_error(const Eigen::VectorXf* output, const Eigen::VectorXf* targets) {
	assert(targets->size() == 1); // We only have one output (the number of cycles)
	double target = (*targets)[0] * MAX_CYCLES;
	double prediction = (*output)[0] * MAX_CYCLES;
	return std::abs(target - prediction);
}

/**
 * @brief Calculate the relative absolute error
 * @param output The output of the network
 * @param targets The target output of the network
 * @return The absolute error
 */
double relative_absolute_error(const Eigen::VectorXf* output, const Eigen::VectorXf* targets) {
	assert(targets->size() == 1); // We only have one output (the number of cycles)
	double target = (*targets)[0] * MAX_CYCLES;
	double prediction = (*output)[0] * MAX_CYCLES;
	std::cout << "Target : " << target << ", Prediction : " << prediction
			  << ", Error : " << std::abs(target - prediction) / std::max(1.0, std::max(target, prediction)) << '\n';
	return std::abs(target - prediction) / std::max(1.0, std::max(target, prediction));
}

NeuralNetwork::NeuralNetwork(std::vector<size_t>& topology, float learning_rate) : m_topology(topology), m_learning_rate(learning_rate) {
	srand(time(nullptr));
	for (size_t i = 0; i < topology.size() - 1; i++) {
		Layer layer(topology[i], topology[i + 1]);
		m_layers.push_back(layer);
	}
	this->m_last_values = Eigen::MatrixXf::Zero(0, 0);
}

void NeuralNetwork::feed_forward(const Eigen::VectorXf* input) {
	if (input->size() != m_topology[0]) {
		throw std::invalid_argument(std::string("Input size does not match the number of weights : " + std::to_string(input->size()) +
												" != " + std::to_string(m_topology[0])));
	}

	Eigen::MatrixXf values2(1, input->size());
	for (long i = 0; i < input->size(); i++) {
		values2(0, i) = (*input)[i];
	}

	// Forwarding inputs to next layers
	Eigen::MatrixXf layer_values = values2;
	for (Layer& layer : m_layers) {
		layer_values = layer.feed_forward(&layer_values);
	}

	// Save the last values
	this->m_last_values = layer_values;
}

void NeuralNetwork::back_propagate(const Eigen::VectorXf* target_output) {
	Eigen::MatrixXf errors(1, target_output->size());
	for (long i = 0; i < target_output->size(); i++) {
		errors(0, i) = (*target_output)[i] - m_last_values(0, i);
	}

	// Propagate for the last values of the layer
	errors = m_layers[m_layers.size() - 1].back_propagate(errors, m_learning_rate, m_last_values);

	// Propagate for the rest of the layers
	for (int i = m_layers.size() - 2; i >= 0; i--) {
		errors = m_layers[i].back_propagate(errors, m_learning_rate, m_layers[i + 1].m_values);
	}
}

Eigen::VectorXf NeuralNetwork::get_prediction(const Eigen::VectorXf* input) {
	// Feed the input through the network
	feed_forward(input);
	// Return the last values (the prediction)
	return m_last_values;
}

std::vector<float> NeuralNetwork::get_prediction(const std::vector<float>& input) {
	auto eigen_vec = eigen_vec_from_cpp_vec(input);
	auto prediction = this->get_prediction(&eigen_vec);
	return cpp_vec_from_eigen_vec(prediction);
}

std::pair<std::vector<Eigen::VectorXf*>, std::vector<Eigen::VectorXf*>>
NeuralNetwork::convert_data_for_loss(std::vector<std::vector<float>>& inputs, std::vector<std::vector<float>>& targets) {
	std::vector<Eigen::VectorXf*> in;
	std::vector<Eigen::VectorXf*> out;
	for (size_t i = 0; i < inputs.size(); i++) {
		auto input = new Eigen::VectorXf(eigen_vec_from_cpp_vec(inputs[i]));
		auto target = new Eigen::VectorXf(eigen_vec_from_cpp_vec(targets[i]));
		in.push_back(input);
		out.push_back(target);
	}
	return {in, out};
}

std::pair<std::vector<Eigen::VectorXf*>, std::vector<Eigen::VectorXf*>>
NeuralNetwork::convert_data_for_loss(std::vector<Eigen::VectorXf>& inputs, std::vector<Eigen::VectorXf>& targets) {
	std::vector<Eigen::VectorXf*> in;
	std::vector<Eigen::VectorXf*> out;
	for (size_t i = 0; i < inputs.size(); i++) {
		in.push_back(&inputs[i]);
		out.push_back(&targets[i]);
	}
	return {in, out};
}

std::pair<std::vector<Eigen::VectorXf*>, std::vector<Eigen::VectorXf*>> NeuralNetwork::convert_data_for_loss(Dataset& dataset) {
	std::vector<Eigen::VectorXf*> in;
	std::vector<Eigen::VectorXf*> out;
	for (auto [input, target] : dataset.get_data(1.0)) {
		in.push_back(&input);
		out.push_back(&target);
	}
	return {in, out};
}

double NeuralNetwork::get_loss_mrse(std::vector<Eigen::VectorXf*>& inputs, std::vector<Eigen::VectorXf*>& targets) {
	double loss = 0;
	for (size_t i = 0; i < inputs.size(); i++) {
		auto prediction = this->get_prediction(inputs[i]);
		loss += relative_squared_error(&prediction, targets[i]);
	}
	return loss / static_cast<double>(inputs.size());
}

double NeuralNetwork::get_loss_mse(std::vector<Eigen::VectorXf*>& inputs, std::vector<Eigen::VectorXf*>& targets) {
	double loss = 0;
	for (size_t i = 0; i < inputs.size(); i++) {
		auto prediction = this->get_prediction(inputs[i]);
		loss += squared_error(&prediction, targets[i]);
	}
	return loss / static_cast<double>(inputs.size());
}

double NeuralNetwork::get_acc_mrae(std::vector<Eigen::VectorXf*>& inputs, std::vector<Eigen::VectorXf*>& targets) {
	double acc = 0;
	for (size_t i = 0; i < inputs.size(); i++) {
		auto prediction = this->get_prediction(inputs[i]);
		acc += relative_absolute_error(&prediction, targets[i]);
	}
	return 1.0 - (acc / static_cast<double>(inputs.size()));
}

double NeuralNetwork::get_acc_mae(std::vector<Eigen::VectorXf*>& inputs, std::vector<Eigen::VectorXf*>& targets) {
	double acc = 0;
	for (size_t i = 0; i < inputs.size(); i++) {
		auto prediction = this->get_prediction(inputs[i]);
		acc += absolute_error(&prediction, targets[i]);
	}
	return 1.0 - (acc / static_cast<double>(inputs.size()));
}

void NeuralNetwork::train(Dataset& dataset, int nb_epochs, float input_ratio, std::string&& logging_filename) {
	// Open the file for logging
	std::ofstream log_file;
	log_file.open(logging_filename);
	auto data = dataset.get_data(1.0);

	// Split the data into training and validation sets
	size_t const train_size = (size_t)(data.size() * input_ratio);
	size_t const validation_size = data.size() - train_size;

	std::cout << "Training size: " << train_size << ", Validation size: " << validation_size << '\n';

	std::vector<Eigen::VectorXf*> train_input_vectors(train_size);
	std::vector<Eigen::VectorXf*> validation_input_vectors(validation_size);
	std::vector<Eigen::VectorXf*> train_target_vectors(train_size);
	std::vector<Eigen::VectorXf*> validation_target_vectors(validation_size);

	for (size_t i = 0; i < data.size(); i++) {
		// Put the input and target in the right set
		if (i < train_size) {
			train_input_vectors[i] = &data[i].first;
			train_target_vectors[i] = &data[i].second;
		}
		else {
			validation_input_vectors[i - train_size] = &data[i].first;
			validation_target_vectors[i - train_size] = &data[i].second;
		}
	}

	// Training the neural network
	const int nb_points_to_plot = (nb_epochs > 1000) ? nb_epochs / 1000 : 1;

	std::chrono::duration<double> total_time = std::chrono::duration<double>::zero();
	for (int i = 0; i < nb_epochs; i++) {
		auto clock = std::chrono::high_resolution_clock::now();
		for (size_t j = 0; j < train_size; j++) {
			std::cout << "Epoch: " << i << " ( " << j << " / " << train_size << " )\n";
			this->feed_forward(train_input_vectors[j]);
			this->back_propagate(train_target_vectors[j]);
		}
		auto end_clock = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed_seconds = end_clock - clock;
		total_time += elapsed_seconds;

		std::cout << "Epoch: " << i << '\n';
		// Logging the loss and accuracy
		if (i % nb_points_to_plot == 0) {
			// Different losses
			double tl_mrse = this->get_loss_mrse(train_input_vectors, train_target_vectors);
			double vl_mrse = this->get_loss_mrse(validation_input_vectors, validation_target_vectors);
			double tl_mse = this->get_loss_mse(train_input_vectors, train_target_vectors);
			double vl_mse = this->get_loss_mse(validation_input_vectors, validation_target_vectors);

			// Different accuracies
			double ta_mae = this->get_acc_mae(train_input_vectors, train_target_vectors);
			double va_mae = this->get_acc_mae(validation_input_vectors, validation_target_vectors);
			double ta_mrae = this->get_acc_mrae(train_input_vectors, train_target_vectors);
			double va_mrae = this->get_acc_mrae(validation_input_vectors, validation_target_vectors);

			std::stringstream stream = std::stringstream();
			stream << "Epoch: " << i << ':';

			stream << "Training loss (MRSE): " << tl_mrse << ',';
			stream << "Validation loss (MRSE): " << vl_mrse << ',';
			stream << "Training loss (MSE): " << tl_mse << ',';
			stream << "Validation loss (MSE): " << vl_mse << ',';

			stream << "Training accuracy (MAE): " << ta_mae << ',';
			stream << "Validation accuracy (MAE): " << va_mae << ',';
			stream << "Training accuracy (MRAE): " << ta_mrae << ',';
			stream << "Validation accuracy (MRAE): " << va_mrae << '\n';

			std::cout << stream.str();
			log_file << stream.str();

			// Print all the values predicted
			// for (auto [input, target] : dataset.get_data(1.0)) {
			// 	auto output = this->get_prediction(&input);
			// 	log_file << "Input: " << input.transpose() << ", Prediction: " << output.transpose() << ", Target: " << target.transpose()
			// 			 << '\n';
			// }
		}
	}

	std::cout << "Training time: " << total_time.count() << "s\n";
	log_file << "Training time: " << total_time.count() << "s\n";
	log_file.close();
}