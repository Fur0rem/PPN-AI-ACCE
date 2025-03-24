/**
 * @file src/neural_network/neural_network.cpp
 * @brief Implementation of the NeuralNetwork class
 */

#include "neural_network/neural_network.hpp"
#include "dataset/dataset.hpp"
#include "parsing/iencoder.hpp"
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <cxxabi.h>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/src/Core/Matrix.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
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

double NeuralNetwork::squared_error(const Eigen::VectorXf* prediction, const Eigen::VectorXf* target, IEncoder* encoder) {
	double error = 0;
	auto prediction_cpp = cpp_vec_from_eigen_vec(*prediction);
	auto target_cpp = cpp_vec_from_eigen_vec(*target);
	auto prediction_decoded = encoder->decode(prediction_cpp);
	auto target_decoded = encoder->decode(target_cpp);
	for (size_t i = 0; i < prediction_decoded.size(); i++) {
		error += (prediction_decoded[i] - target_decoded[i]) * (prediction_decoded[i] - target_decoded[i]);
	}
	return error / prediction_decoded.size();
}

double NeuralNetwork::relative_squared_error(const Eigen::VectorXf* prediction, const Eigen::VectorXf* target, IEncoder* encoder) {
	double error = 0;
	auto prediction_cpp = cpp_vec_from_eigen_vec(*prediction);
	auto target_cpp = cpp_vec_from_eigen_vec(*target);
	auto prediction_decoded = encoder->decode(prediction_cpp);
	auto target_decoded = encoder->decode(target_cpp);
	for (size_t i = 0; i < prediction_decoded.size(); i++) {
		auto target_i = target_decoded[i];
		auto prediction_i = prediction_decoded[i];
		auto diff = target_i - prediction_i;
		error += diff * diff / std::max(1.0F, std::max(target_i * target_i, prediction_i * prediction_i));
	}
	return error / prediction_decoded.size();
}

double NeuralNetwork::absolute_error(const Eigen::VectorXf* prediction, const Eigen::VectorXf* target, IEncoder* encoder) {
	double error = 0;
	auto prediction_cpp = cpp_vec_from_eigen_vec(*prediction);
	auto target_cpp = cpp_vec_from_eigen_vec(*target);
	auto prediction_decoded = encoder->decode(prediction_cpp);
	auto target_decoded = encoder->decode(target_cpp);
	for (size_t i = 0; i < prediction_decoded.size(); i++) {
		error += std::abs(prediction_decoded[i] - target_decoded[i]);
	}
	return error / prediction_decoded.size();
}

double NeuralNetwork::relative_absolute_error(const Eigen::VectorXf* prediction, const Eigen::VectorXf* target, IEncoder* encoder) {
	double error = 0;
	auto prediction_cpp = cpp_vec_from_eigen_vec(*prediction);
	auto target_cpp = cpp_vec_from_eigen_vec(*target);
	auto prediction_decoded = encoder->decode(prediction_cpp);
	auto target_decoded = encoder->decode(target_cpp);
	for (size_t i = 0; i < prediction_decoded.size(); i++) {
		auto target_i = target_decoded[i];
		auto prediction_i = prediction_decoded[i];
		auto diff = target_i - prediction_i;
		error += std::abs(diff) / std::max(1.0F, std::max(target_i, prediction_i));
	}
	return error / prediction_decoded.size();
}

NeuralNetwork::NeuralNetwork(std::vector<size_t>& topology, float learning_rate, std::unique_ptr<ActivationFunc> activation_func)
	: m_topology(topology), m_learning_rate(learning_rate) {

	// Initialize the layers
	srand(time(nullptr));
	for (size_t i = 0; i < topology.size() - 1; i++) {
		Layer layer(topology[i], topology[i + 1]);
		m_layers.push_back(layer);
	}
	// Initialize the last values (empty for now)
	this->m_last_values = Eigen::MatrixXf::Zero(0, 0);

	// Set the activation function
	ActivationFunc* activation_func_ptr = activation_func.get();
	std::function<float(float)> activation_func_f = [activation_func_ptr](float x) {
		return activation_func_ptr->func(x);
	};
	std::function<float(float)> activation_func_deriv_f = [activation_func_ptr](float x) {
		return activation_func_ptr->deriv(x);
	};
	Layer::set_activation_func(activation_func_f, activation_func_deriv_f);

	this->m_activation_func = std::move(activation_func);
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
	Eigen::VectorXf prediction(m_last_values.cols());
	for (long i = 0; i < m_last_values.cols(); i++) {
		prediction[i] = m_last_values(0, i);
	}
	return prediction;
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
	for (auto [_, input, target] : dataset.get_data(1.0)) {
		in.push_back(&input);
		out.push_back(&target);
	}
	return {in, out};
}

double NeuralNetwork::get_loss_mrse(std::vector<Eigen::VectorXf*>& inputs, std::vector<Eigen::VectorXf*>& targets, IEncoder* encoder) {
	double loss = 0;
	for (size_t i = 0; i < inputs.size(); i++) {
		auto prediction = this->get_prediction(inputs[i]);
		loss += relative_squared_error(&prediction, targets[i], encoder);
	}
	return loss / static_cast<double>(inputs.size());
}

double NeuralNetwork::get_loss_mse(std::vector<Eigen::VectorXf*>& inputs, std::vector<Eigen::VectorXf*>& targets, IEncoder* encoder) {
	double loss = 0;
	for (size_t i = 0; i < inputs.size(); i++) {
		auto prediction = this->get_prediction(inputs[i]);
		loss += squared_error(&prediction, targets[i], encoder);
	}
	return loss / static_cast<double>(inputs.size());
}

double NeuralNetwork::get_acc_mrae(std::vector<Eigen::VectorXf*>& inputs, std::vector<Eigen::VectorXf*>& targets, IEncoder* encoder) {
	double acc = 0;
	for (size_t i = 0; i < inputs.size(); i++) {
		auto prediction = this->get_prediction(inputs[i]);
		acc += relative_absolute_error(&prediction, targets[i], encoder);
	}
	return 1.0 - (acc / static_cast<double>(inputs.size()));
}

double NeuralNetwork::get_acc_mae(std::vector<Eigen::VectorXf*>& inputs, std::vector<Eigen::VectorXf*>& targets, IEncoder* encoder) {
	double acc = 0;
	for (size_t i = 0; i < inputs.size(); i++) {
		auto prediction = this->get_prediction(inputs[i]);
		acc += absolute_error(&prediction, targets[i], encoder);
	}
	return 1.0 - (acc / static_cast<double>(inputs.size()));
}

void NeuralNetwork::train(Dataset& dataset, int nb_epochs, float training_proportion, std::string&& logging_dir, int nb_trains) {

	// Create the logging directory
	std::filesystem::create_directory(logging_dir);

	for (int t = 1; t <= nb_trains; t++) {

		// Open the file for logging
		std::string logging_filename = logging_dir + "/train_" + std::to_string(t) + ".log";
		std::ofstream log_file;
		log_file.open(logging_filename);
		auto data = dataset.get_data(1.0);

		// Split the data into training and validation sets
		size_t const train_size = (size_t)((double)data.size() * training_proportion);
		size_t const validation_size = data.size() - train_size;

		std::cout << "Training size: " << train_size << ", Validation size: " << validation_size << '\n';

		std::vector<Eigen::VectorXf*> train_input_vectors(train_size);
		std::vector<Eigen::VectorXf*> validation_input_vectors(validation_size);
		std::vector<Eigen::VectorXf*> train_target_vectors(train_size);
		std::vector<Eigen::VectorXf*> validation_target_vectors(validation_size);
		std::vector<std::string> train_names(train_size);
		std::vector<std::string> validation_names(validation_size);

		for (size_t i = 0; i < data.size(); i++) {
			// Put the input and target in the right set
			if (i < train_size) {
				train_names[i] = std::get<0>(data[i]);
				train_input_vectors[i] = &std::get<1>(data[i]);
				train_target_vectors[i] = &std::get<2>(data[i]);
			}
			else {
				validation_names[i - train_size] = std::get<0>(data[i]);
				validation_input_vectors[i - train_size] = &std::get<1>(data[i]);
				validation_target_vectors[i - train_size] = &std::get<2>(data[i]);
			}
		}

		// Training the neural network
		const int nb_points_to_plot = (nb_epochs > 1000) ? nb_epochs / 1000 : 1;

		std::chrono::duration<double> total_time = std::chrono::duration<double>::zero();
		for (int i = 0; i < nb_epochs; i++) {
			auto clock = std::chrono::high_resolution_clock::now();
			for (size_t j = 0; j < train_size; j++) {
				// std::cout << "Epoch: " << i << " ( " << j + 1 << " / " << train_size << " )\n";
				this->feed_forward(train_input_vectors[j]);
				this->back_propagate(train_target_vectors[j]);
			}
			auto end_clock = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed_seconds = end_clock - clock;
			total_time += elapsed_seconds;

			// std::cout << "Epoch: " << i << '\n';

			// Logging the loss and accuracy
			if (i % nb_points_to_plot == 0) {
				// Different losses
				double tl_mrse = this->get_loss_mrse(train_input_vectors, train_target_vectors, dataset.get_output_encoder());
				double vl_mrse = this->get_loss_mrse(validation_input_vectors, validation_target_vectors, dataset.get_output_encoder());
				double tl_mse = this->get_loss_mse(train_input_vectors, train_target_vectors, dataset.get_output_encoder());
				double vl_mse = this->get_loss_mse(validation_input_vectors, validation_target_vectors, dataset.get_output_encoder());

				// Different accuracies
				double ta_mae = this->get_acc_mae(train_input_vectors, train_target_vectors, dataset.get_output_encoder());
				double va_mae = this->get_acc_mae(validation_input_vectors, validation_target_vectors, dataset.get_output_encoder());
				double ta_mrae = this->get_acc_mrae(train_input_vectors, train_target_vectors, dataset.get_output_encoder());
				double va_mrae = this->get_acc_mrae(validation_input_vectors, validation_target_vectors, dataset.get_output_encoder());

				std::stringstream stream = std::stringstream();
				stream << "Epoch: " << i << ":\t";

				stream << "Training loss (MRSE): " << tl_mrse << "\t,";
				stream << "Validation loss (MRSE): " << vl_mrse << "\t,";
				stream << "Training loss (MSE): " << tl_mse << "\t,";
				stream << "Validation loss (MSE): " << vl_mse << "\t,";

				stream << "Training accuracy (MAE): " << ta_mae << "\t,";
				stream << "Validation accuracy (MAE): " << va_mae << "\t,";
				stream << "Training accuracy (MRAE): " << ta_mrae << "\t,";
				stream << "Validation accuracy (MRAE): " << va_mrae << '\n';

				// std::cout << stream.str();
				log_file << stream.str();
			}
			// One time every 10 points, flush the log file to avoid losing data
			if (i % (nb_points_to_plot * 10) == 0) {
				log_file.flush();
			}
		}

		std::cout << "Training time: " << total_time.count() << "s\n";
		log_file << "Training time: " << total_time.count() << "s\n\n";

		log_file << "Final results:\n";

		log_file << "Training data:\n";
		for (size_t j = 0; j < train_size; j++) {
			auto prediction = this->get_prediction(train_input_vectors[j]);
			auto prediction_cpp = cpp_vec_from_eigen_vec(prediction);
			auto target_cpp = cpp_vec_from_eigen_vec(*train_target_vectors[j]);
			auto prediction_decoded = dataset.get_output_encoder()->decode(prediction_cpp);
			auto target_decoded = dataset.get_output_encoder()->decode(target_cpp);
			log_file << "For " << train_names[j] << ":\t";
			log_file << "Prediction: \t";
			for (auto val : prediction_decoded) {
				log_file << val << ' ';
			}
			log_file << "Target: \t";
			for (auto val : target_decoded) {
				log_file << val << ' ';
			}
			log_file << '\n';
		}

		log_file << "\nValidation data:\n";
		for (size_t j = 0; j < validation_size; j++) {
			auto prediction = this->get_prediction(validation_input_vectors[j]);
			auto prediction_cpp = cpp_vec_from_eigen_vec(prediction);
			auto target_cpp = cpp_vec_from_eigen_vec(*validation_target_vectors[j]);
			auto prediction_decoded = dataset.get_output_encoder()->decode(prediction_cpp);
			auto target_decoded = dataset.get_output_encoder()->decode(target_cpp);
			log_file << "For " << validation_names[j] << ":\t";
			log_file << "Prediction: \t";
			for (auto val : prediction_decoded) {
				log_file << val << ' ';
			}
			log_file << "Target: \t";
			for (auto val : target_decoded) {
				log_file << val << ' ';
			}
			log_file << '\n';
		}

		log_file << "\nNetwork topology:\n";
		for (auto val : m_topology) {
			log_file << val << ' ';
		}
		log_file << '\n';
		log_file << "Learning rate: " << m_learning_rate << '\n';
		char* activation_func_name = abi::__cxa_demangle(typeid(*this->m_activation_func).name(), 0, 0, nullptr);
		log_file << "Activation function: " << activation_func_name << '\n';
		free(activation_func_name);
		log_file.close();
	}
}

void NeuralNetwork::back_propagate_batch(std::vector<Eigen::VectorXf*>& predictions, std::vector<Eigen::VectorXf*>& targets,
										 float learning_rate) {
	// Calculate the gradients
	Eigen::MatrixXf prev_errors = Eigen::MatrixXf::Zero(1, targets[0]->size());
	for (size_t i = 0; i < predictions.size(); i++) {
		Eigen::MatrixXf errors(1, targets[i]->size());
		// Compute the errors for the current prediction
		for (long j = 0; j < targets[i]->size(); j++) {
			errors(0, j) = (*targets[i])(j) - (*predictions[i])(j);
		}
		prev_errors += errors;
	}

	// Average the errors over the predictions
	prev_errors /= static_cast<float>(predictions.size());

	// Propagate for the last values of the layer
	prev_errors = m_layers[m_layers.size() - 1].back_propagate(prev_errors, learning_rate, m_last_values);

	// Propagate for the rest of the layers
	for (int i = m_layers.size() - 2; i >= 0; i--) {
		prev_errors = m_layers[i].back_propagate(prev_errors, learning_rate, m_layers[i + 1].m_values);
	}
}

void NeuralNetwork::train_batch(Dataset& dataset, int nb_epochs, float training_proportion, float rolling_proportion,
								std::string&& logging_dir, int nb_trains) {

	// Create the logging directory
	std::filesystem::create_directory(logging_dir);

	for (int t = 1; t <= nb_trains; t++) {

		// Open the file for logging
		std::string logging_filename = logging_dir + "/train_" + std::to_string(t) + ".log";
		std::ofstream log_file;
		log_file.open(logging_filename);
		auto data = dataset.get_data(1.0);

		// Split the data into training and validation sets
		size_t const train_size = (size_t)((double)data.size() * training_proportion);
		size_t const validation_size = data.size() - train_size;

		std::cout << "Training size: " << train_size << ", Validation size: " << validation_size << '\n';

		std::vector<Eigen::VectorXf*> train_input_vectors(train_size);
		std::vector<Eigen::VectorXf*> validation_input_vectors(validation_size);
		std::vector<Eigen::VectorXf*> train_target_vectors(train_size);
		std::vector<Eigen::VectorXf*> validation_target_vectors(validation_size);
		std::vector<std::string> train_names(train_size);
		std::vector<std::string> validation_names(validation_size);

		for (size_t i = 0; i < data.size(); i++) {
			// Put the input and target in the right set
			if (i < train_size) {
				train_names[i] = std::get<0>(data[i]);
				train_input_vectors[i] = &std::get<1>(data[i]);
				train_target_vectors[i] = &std::get<2>(data[i]);
			}
			else {
				validation_names[i - train_size] = std::get<0>(data[i]);
				validation_input_vectors[i - train_size] = &std::get<1>(data[i]);
				validation_target_vectors[i - train_size] = &std::get<2>(data[i]);
			}
		}

		// Training the neural network
		const int nb_points_to_plot = (nb_epochs > 1000) ? nb_epochs / 1000 : 1;

		std::chrono::duration<double> total_time = std::chrono::duration<double>::zero();

		size_t rolling_train_size = (size_t)(train_size * rolling_proportion);
		std::vector<size_t> rolling_indices(train_size);
		for (size_t i = 0; i < train_size; i++) {
			rolling_indices[i] = i;
		}

		std::cout << "Rolling training size: " << rolling_train_size << '\n';

		// Batch training
		for (int i = 0; i < nb_epochs; i++) {
			auto clock = std::chrono::high_resolution_clock::now();
			std::vector<Eigen::VectorXf> predictions;
			std::vector<Eigen::VectorXf> targets;
			// Shuffle the indices
			std::shuffle(rolling_indices.begin(), rolling_indices.end(), std::mt19937(std::random_device()()));

			// Make the predictions and targets
			for (size_t j = 0; j < rolling_train_size; j++) {
				// std::cout << "Epoch: " << i << " ( " << j + 1 << " / " << train_size << " )\n";
				this->feed_forward(train_input_vectors[rolling_indices[j]]);
				predictions.push_back(m_last_values);
				targets.push_back(*train_target_vectors[rolling_indices[j]]);
			}

			// Back propagate the batch
			auto [predictions_ptr, targets_ptr] = convert_data_for_loss(predictions, targets);
			this->back_propagate_batch(predictions_ptr, targets_ptr, m_learning_rate);

			auto end_clock = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed_seconds = end_clock - clock;
			total_time += elapsed_seconds;

			// std::cout << "Epoch: " << i << '\n';

			// Logging the loss and accuracy
			if (i % nb_points_to_plot == 0) {
				// Different losses
				double tl_mrse = this->get_loss_mrse(train_input_vectors, train_target_vectors, dataset.get_output_encoder());
				double vl_mrse = this->get_loss_mrse(validation_input_vectors, validation_target_vectors, dataset.get_output_encoder());
				double tl_mse = this->get_loss_mse(train_input_vectors, train_target_vectors, dataset.get_output_encoder());
				double vl_mse = this->get_loss_mse(validation_input_vectors, validation_target_vectors, dataset.get_output_encoder());

				// Different accuracies
				double ta_mae = this->get_acc_mae(train_input_vectors, train_target_vectors, dataset.get_output_encoder());
				double va_mae = this->get_acc_mae(validation_input_vectors, validation_target_vectors, dataset.get_output_encoder());
				double ta_mrae = this->get_acc_mrae(train_input_vectors, train_target_vectors, dataset.get_output_encoder());
				double va_mrae = this->get_acc_mrae(validation_input_vectors, validation_target_vectors, dataset.get_output_encoder());

				std::stringstream stream = std::stringstream();
				stream << "Epoch: " << i << ":\t";

				stream << "Training loss (MRSE): " << tl_mrse << "\t,";
				stream << "Validation loss (MRSE): " << vl_mrse << "\t,";
				stream << "Training loss (MSE): " << tl_mse << "\t,";
				stream << "Validation loss (MSE): " << vl_mse << "\t,";

				stream << "Training accuracy (MAE): " << ta_mae << "\t,";
				stream << "Validation accuracy (MAE): " << va_mae << "\t,";
				stream << "Training accuracy (MRAE): " << ta_mrae << "\t,";
				stream << "Validation accuracy (MRAE): " << va_mrae << '\n';

				// std::cout << stream.str();
				log_file << stream.str();
			}
			// One time every 10 points, flush the log file to avoid losing data
			if (i % (nb_points_to_plot * 10) == 0) {
				log_file.flush();
			}
		}

		std::cout << "Training time: " << total_time.count() << "s\n";

		log_file << "Training time: " << total_time.count() << "s\n\n";

		log_file << "Final results:\n";

		log_file << "Training data:\n";
		for (size_t j = 0; j < train_size; j++) {
			auto prediction = this->get_prediction(train_input_vectors[j]);
			auto prediction_cpp = cpp_vec_from_eigen_vec(prediction);
			auto target_cpp = cpp_vec_from_eigen_vec(*train_target_vectors[j]);
			auto prediction_decoded = dataset.get_output_encoder()->decode(prediction_cpp);
			auto target_decoded = dataset.get_output_encoder()->decode(target_cpp);
			log_file << "For " << train_names[j] << ":\t";
			log_file << "Prediction: \t";
			for (auto val : prediction_decoded) {
				log_file << val << ' ';
			}
			log_file << "Target: \t";
			for (auto val : target_decoded) {
				log_file << val << ' ';
			}
			log_file << '\n';
		}

		log_file << "\nValidation data:\n";
		for (size_t j = 0; j < validation_size; j++) {
			auto prediction = this->get_prediction(validation_input_vectors[j]);
			auto prediction_cpp = cpp_vec_from_eigen_vec(prediction);
			auto target_cpp = cpp_vec_from_eigen_vec(*validation_target_vectors[j]);
			auto prediction_decoded = dataset.get_output_encoder()->decode(prediction_cpp);
			auto target_decoded = dataset.get_output_encoder()->decode(target_cpp);
			log_file << "For " << validation_names[j] << ":\t";
			log_file << "Prediction: \t";
			for (auto val : prediction_decoded) {
				log_file << val << ' ';
			}
			log_file << "Target: \t";
			for (auto val : target_decoded) {
				log_file << val << ' ';
			}
			log_file << '\n';
		}

		log_file << "\nNetwork topology:\n";
		for (auto val : m_topology) {
			log_file << val << ' ';
		}
		log_file << '\n';
		log_file << "Learning rate: " << m_learning_rate << '\n';
		char* activation_func_name = abi::__cxa_demangle(typeid(*this->m_activation_func).name(), 0, 0, nullptr);
		log_file << "Activation function: " << activation_func_name << '\n';
		free(activation_func_name);
		log_file.close();
	}
}