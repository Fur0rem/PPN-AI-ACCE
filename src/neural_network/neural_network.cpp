/**
 * @file src/neural_network/neural_network.cpp
 * @brief Implementation of the NeuralNetwork class
 */

#include "neural_network/neural_network.hpp"
#include "neural_network/activation_functions.hpp"
#include "neural_network/gradients.hpp"
#include "neural_network/optimiser.hpp"
#include <cmath>
#include <cstdlib>
#include <cxxabi.h>
#include <eigen3/Eigen/Dense>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

std::vector<float> get_row(const Eigen::MatrixXf& matrix, size_t row) {
	std::vector<float> result(matrix.cols());
	for (size_t i = 0; i < matrix.cols(); i++) {
		result[i] = matrix(row, i);
	}
	return result;
}

NeuralNetwork::NeuralNetwork(const std::vector<size_t>& topology, std::unique_ptr<ActivationFunc> activation_func)
	: m_topology(topology), m_activation_func(std::move(activation_func)) {
	for (size_t i = 0; i < topology.size() - 1; i++) {
		// Initialize weights with small random values
		m_weights.push_back(Eigen::MatrixXf(topology[i], topology[i + 1]));
		for (size_t j = 0; j < topology[i]; j++) {
			for (size_t k = 0; k < topology[i + 1]; k++) {
				// In interval [-0.01, 0.01]
				m_weights[i](j, k) = 0.02F * (static_cast<float>(rand()) / RAND_MAX) - 0.01F;

				// Make sure it's not zero (or close to zero)
				if (std::abs(m_weights[i](j, k)) < 0.0001F) {
					m_weights[i](j, k) = 0.01F;
				}
			}
		}

		// Initialize biases to zero
		m_biases.push_back(Eigen::MatrixXf::Zero(1, topology[i + 1]));
	}
}

std::vector<float> NeuralNetwork::predict(const std::vector<float>& input) {
	Eigen::MatrixXf input_matrix(1, input.size());
	for (size_t i = 0; i < input.size(); i++) {
		input_matrix(0, i) = input[i];
	}
	Eigen::MatrixXf output = this->feed_forward(input_matrix, 0.0f);
	std::vector<float> result(output.cols());
	for (size_t i = 0; i < output.cols(); i++) {
		result[i] = output(0, i);
	}
	return result;
}

Eigen::MatrixXf NeuralNetwork::predict(const Eigen::MatrixXf& input) {
	Eigen::MatrixXf output = this->feed_forward(input, 0.0f);
	return output;
}

Eigen::MatrixXf NeuralNetwork::feed_forward(const Eigen::MatrixXf& input, float dropout_rate) {

	for (size_t i = 0; i < m_topology.size() - 1; i++) {
		// get matrix number of row to know the number of neuron
		//  choose dropout neuron number  dpn=  floor(rate*number_neuron) to know how much neuon you need to shut down
		// choose randomly
		//  create a random vector dpv of col size with dpn number of zero entries and the rest zero
		//  calculate the matrix multiplication normaly as usual
		//  then at the end before sending those calculate value we multiply the dpv vector with the  a(j,k)
		//  float dropout_rate = 0.2;
		//  int dpn  =   floor(dropout_rate*m_weights.cols());
		int col_size = m_weights[i].cols();

		int dpn = static_cast<int>(std::floor(dropout_rate * col_size));
		std::cout << "the columns size is " << m_weights[i].cols() << " the number of dropout neuron will be " << dpn << "\n";

		// fill the vector with 1
		std::vector<float> vec(col_size, 1.0f);
		// fill dpn entry with zero
		std::fill_n(vec.begin(), dpn, 0.0f);
		// shuffle the zero in the vector
		std::shuffle(vec.begin(), vec.end(), std::mt19937{std::random_device{}()});
		// convert std vector to eigen
		Eigen::VectorXf dpv = Eigen::Map<Eigen::VectorXf>(vec.data(), col_size);

		m_a_values.clear();
		m_z_values.clear();
		m_a_values.push_back(input);

		Eigen::MatrixXf last_a = m_a_values.back();
		// Eigen::MatrixXf z = last_a.dot(m_weights[i]);
		Eigen::MatrixXf z = last_a * m_weights[i];
		for (size_t j = 0; j < z.rows(); j++) {
			for (size_t k = 0; k < z.cols(); k++) {
				z(j, k) += m_biases[i](0, k);
			}
		}
		m_z_values.push_back(z);
		Eigen::MatrixXf a(z.rows(), z.cols());
		for (size_t j = 0; j < z.rows(); j++) {
			for (size_t k = 0; k < z.cols(); k++) {
				a(j, k) = m_activation_func->func(z(j, k));
			}
		}
		// dropout mask applied to the batch of output (one output per row )
		a = a.array().rowwise() * dpv.transpose().array();
		m_a_values.push_back(a);
	}

	return m_a_values.back();
}

Gradients NeuralNetwork::backward(const Eigen::MatrixXf& inputs, const Eigen::MatrixXf& targets) {
	size_t m = inputs.rows();
	Eigen::MatrixXf predictions = m_a_values.back();
	Eigen::MatrixXf loss_grad = mae_loss_derivative(predictions, targets);

	Gradients grads(0);
	for (ssize_t i = (ssize_t)m_topology.size() - 2; i >= 0; i--) {
		// Compute the derivative of the activation function
		Eigen::MatrixXf derivative(m_z_values[i].rows(), m_z_values[i].cols());
		for (size_t j = 0; j < m_z_values[i].rows(); j++) {
			for (size_t k = 0; k < m_z_values[i].cols(); k++) {
				derivative(j, k) = m_activation_func->deriv(m_z_values[i](j, k));
			}
		}

		// Compute the gradients
		Eigen::MatrixXf dz = loss_grad.cwiseProduct(derivative);
		Eigen::MatrixXf dw = m_a_values[i].transpose() * dz;
		dw /= (float)m;
		Eigen::MatrixXf db(1, dz.cols());
		for (size_t j = 0; j < dz.cols(); j++) {
			db(0, j) = 0.0f;
			for (size_t k = 0; k < dz.rows(); k++) {
				db(0, j) += dz(k, j);
			}
			db(0, j) /= (float)m;
		}
		loss_grad = dz * m_weights[i].transpose();
		grads.d_w().insert(grads.d_w().begin(), dw);
		grads.d_b().insert(grads.d_b().begin(), db);
	}
	return grads;
}

std::pair<float, float> NeuralNetwork::train(Dataset& dataset, size_t nb_epochs, float training_proportion, float learning_rate,
											 std::string&& logging_dir, size_t nb_trains, float dropout_rate) {
	// Create the logging directory
	std::filesystem::create_directory(logging_dir);

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

	Eigen::MatrixXf train_input_matrix(train_size, train_input_vectors[0]->size());
	Eigen::MatrixXf train_target_matrix(train_size, train_target_vectors[0]->size());

	// Check for empty validation set
	size_t matrix_y_for_validation;
	if (validation_size > 0) {
		matrix_y_for_validation = validation_target_vectors[0]->size();
	}
	else {
		matrix_y_for_validation = 0;
	}
	Eigen::MatrixXf validation_input_matrix(validation_size, matrix_y_for_validation);
	Eigen::MatrixXf validation_target_matrix(validation_size, matrix_y_for_validation);
	for (size_t j = 0; j < train_size; j++) {
		for (size_t k = 0; k < train_input_vectors[0]->size(); k++) {
			train_input_matrix(j, k) = (*train_input_vectors[j])(k);
		}
		for (size_t k = 0; k < train_target_vectors[0]->size(); k++) {
			train_target_matrix(j, k) = (*train_target_vectors[j])(k);
		}
	}
	for (size_t j = 0; j < validation_size; j++) {
		for (size_t k = 0; k < validation_input_vectors[0]->size(); k++) {
			validation_input_matrix(j, k) = (*validation_input_vectors[j])(k);
		}
		for (size_t k = 0; k < validation_target_vectors[0]->size(); k++) {
			validation_target_matrix(j, k) = (*validation_target_vectors[j])(k);
		}
	}

	for (size_t t = 1; t <= nb_trains; t++) {
		// Reset the neural network
		this->reset();

		// Open the file for logging
		std::string logging_filename = logging_dir + "/train_" + std::to_string(t) + ".log";
		std::ofstream log_file;
		log_file.open(logging_filename);

		// Training the neural network
		const int nb_points_to_plot = (nb_epochs > 1000) ? nb_epochs / 1000 : 1;

		std::chrono::duration<double> total_time = std::chrono::duration<double>::zero();
		for (int i = 0; i < nb_epochs; i++) {
			auto clock = std::chrono::high_resolution_clock::now();
			for (size_t j = 0; j < train_size; j++) {
				Eigen::VectorXf* input = train_input_vectors[j];
				Eigen::VectorXf* target = train_target_vectors[j];
				// Forward pass
				Eigen::MatrixXf input_matrix(1, input->size());
				for (size_t k = 0; k < input->size(); k++) {
					input_matrix(0, k) = (*input)(k);
				}
				Eigen::MatrixXf target_matrix(1, target->size());
				for (size_t k = 0; k < target->size(); k++) {
					target_matrix(0, k) = (*target)(k);
				}
				// Eigen::MatrixXf output = this->feed_forward(input_matrix);
				Eigen::MatrixXf output = this->feed_forward(input_matrix, dropout_rate);
				// Backward pass
				Gradients grads = this->backward(input_matrix, target_matrix);
				// Update weights and biases
				Eigen::MatrixXf errors(1, target->size());
				for (size_t k = 0; k < target->size(); k++) {
					errors(0, k) = (*target)(k)-output(0, k);
				}
				// Update weights and biases
				for (size_t k = 0; k < m_topology.size() - 1; k++) {
					m_weights[k] = m_weights[k] - (grads.d_w()[k] * learning_rate);
					m_biases[k] = m_biases[k] - (grads.d_b()[k] * learning_rate);
				}
			}
			auto end_clock = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed_seconds = end_clock - clock;
			total_time += elapsed_seconds;

			// Logging the loss and accuracy
			if (i % nb_points_to_plot == 0) {

				this->log_epoch_metrics(i,
										train_input_matrix,
										train_target_matrix,
										validation_input_matrix,
										validation_target_matrix,
										dataset,
										log_file,
										logging_filename);
			}
			// One time every 10 points, flush the log file to avoid losing data
			if (i % (nb_points_to_plot * 10) == 0) {
				log_file.flush();
			}
		}

		std::cout << "Training time: " << total_time.count() << "s\n";

		IOptimiser* opt = new SGD(learning_rate);
		this->log_final_results(log_file,
								dataset,
								train_input_matrix,
								train_target_matrix,
								train_names,
								validation_input_matrix,
								validation_target_matrix,
								validation_names,
								train_size,
								validation_size,
								total_time,
								*opt);
		delete opt;
		log_file.close();
	}

	// Evaluate the model (accuracy of the training and validation sets)
	double train_acc = get_acc_mrae(train_input_matrix, train_target_matrix, dataset.get_output_encoder());
	double validation_acc = get_acc_mrae(validation_input_matrix, validation_target_matrix, dataset.get_output_encoder());
	return std::make_pair(train_acc, validation_acc);
}

std::pair<float, float> NeuralNetwork::train_batch(Dataset& dataset, size_t nb_epochs, float training_proportion, size_t batch_size,
												   IOptimiser& optimiser, std::string&& logging_dir, size_t nb_trains, float dropout_rate) {

	// Create the logging directory
	std::filesystem::create_directory(logging_dir);

	auto data = dataset.get_all_data();
	std::vector<std::vector<float>> inputs;
	std::vector<std::vector<float>> targets;

	for (auto& d : data) {
		auto input = std::get<1>(d);
		auto target = std::get<2>(d);
		inputs.push_back(std::vector<float>());
		targets.push_back(std::vector<float>());
		for (size_t i = 0; i < input.size(); i++) {
			inputs.back().push_back(input[i]);
		}
		for (size_t i = 0; i < target.size(); i++) {
			targets.back().push_back(target[i]);
		}
	}

	const size_t train_size = (size_t)((double)data.size() * training_proportion);
	const size_t validation_size = data.size() - train_size;

	Eigen::MatrixXf train_inputs_mat(train_size, inputs[0].size());
	Eigen::MatrixXf train_targets_mat(train_size, targets[0].size());
	std::vector<std::string> train_names(train_size);
	Eigen::MatrixXf validation_inputs_mat(validation_size, inputs[0].size());
	Eigen::MatrixXf validation_targets_mat(validation_size, targets[0].size());
	std::vector<std::string> validation_names(validation_size);

	for (size_t i = 0; i < train_size; i++) {
		for (size_t j = 0; j < inputs[0].size(); j++) {
			train_inputs_mat(i, j) = inputs[i][j];
		}
		for (size_t j = 0; j < targets[0].size(); j++) {
			train_targets_mat(i, j) = targets[i][j];
		}
		train_names[i] = std::get<0>(data[i]);
	}

	for (size_t i = 0; i < validation_size; i++) {
		for (size_t j = 0; j < inputs[0].size(); j++) {
			validation_inputs_mat(i, j) = inputs[i + train_size][j];
		}
		for (size_t j = 0; j < targets[0].size(); j++) {
			validation_targets_mat(i, j) = targets[i + train_size][j];
		}
		validation_names[i] = std::get<0>(data[i + train_size]);
	}

	for (size_t train = 0; train < nb_trains; train++) {
		std::chrono::duration<double> total_time = std::chrono::duration<double>::zero();
		// Reset the neural network to avoid using the previous weights
		this->reset();

		// Open the file for logging
		std::string logging_filename = logging_dir + "/train_" + std::to_string(train) + ".log";
		std::ofstream log_file;
		log_file.open(logging_filename);

		for (size_t epoch = 0; epoch < nb_epochs; epoch++) {
			// Shuffle data
			auto clock = std::chrono::high_resolution_clock::now();

			std::vector<size_t> indices(train_inputs_mat.rows());
			for (size_t i = 0; i < train_inputs_mat.rows(); i++) {
				indices[i] = i;
			}
			std::shuffle(indices.begin(), indices.end(), std::mt19937(std::random_device()()));

			Eigen::MatrixXf x_shuffled(train_inputs_mat.rows(), train_inputs_mat.cols());
			Eigen::MatrixXf y_shuffled(train_targets_mat.rows(), train_targets_mat.cols());
			for (size_t i = 0; i < train_inputs_mat.rows(); i++) {
				for (size_t j = 0; j < train_inputs_mat.cols(); j++) {
					x_shuffled(i, j) = train_inputs_mat(indices[i], j);
				}
			}
			for (size_t i = 0; i < train_targets_mat.rows(); i++) {
				for (size_t j = 0; j < train_targets_mat.cols(); j++) {
					y_shuffled(i, j) = train_targets_mat(indices[i], j);
				}
			}

			// Mini-batch gradient descent
			for (size_t i = 0; i < train_inputs_mat.rows(); i += batch_size) {
				size_t end = std::min(i + batch_size, (size_t)train_inputs_mat.rows());
				Eigen::MatrixXf x_batch(end - i, train_inputs_mat.cols());
				Eigen::MatrixXf y_batch(end - i, train_targets_mat.cols());
				for (size_t j = i; j < end; j++) {
					for (size_t k = 0; k < train_inputs_mat.cols(); k++) {
						x_batch(j - i, k) = x_shuffled(j, k);
					}
					for (size_t k = 0; k < train_targets_mat.cols(); k++) {
						y_batch(j - i, k) = y_shuffled(j, k);
					}
				}

				// Forward pass
				this->feed_forward(x_batch, dropout_rate);

				// Backward pass
				Gradients grads = this->backward(x_batch, y_batch);

				// Update weights
				optimiser.update_weights(grads, *this);
			}

			auto end_clock = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed_seconds = end_clock - clock;
			total_time += elapsed_seconds;

			const int nb_points_to_plot = (nb_epochs > 1000) ? nb_epochs / 1000 : 1;
			if (epoch % nb_points_to_plot == 0) {
				this->log_epoch_metrics(epoch,
										train_inputs_mat,
										train_targets_mat,
										validation_inputs_mat,
										validation_targets_mat,
										dataset,
										log_file,
										logging_filename);
			}
			// One time every 10 points, flush the log file to avoid losing data
			if (epoch % (nb_points_to_plot * 10) == 0) {
				log_file.flush();
			}
		}
		std::cout << "Training time: " << total_time.count() << "s\n";
		this->log_final_results(log_file,
								dataset,
								train_inputs_mat,
								train_targets_mat,
								train_names,
								validation_inputs_mat,
								validation_targets_mat,
								validation_names,
								train_size,
								validation_size,
								total_time,
								optimiser);

		log_file.close();
	}

	// Evaluate the model (accuracy of the training and validation sets)
	double train_acc = get_acc_mrae(train_inputs_mat, train_targets_mat, dataset.get_output_encoder());
	double validation_acc = get_acc_mrae(validation_inputs_mat, validation_targets_mat, dataset.get_output_encoder());
	return std::make_pair(train_acc, validation_acc);
}

double NeuralNetwork::squared_error(std::vector<float>& prediction, std::vector<float>& target, const IEncoder* encoder) {
	auto prediction_decoded = encoder->decode(prediction);
	auto target_decoded = encoder->decode(target);
	double error = 0.0;
	for (size_t i = 0; i < prediction_decoded.size(); i++) {
		error += std::pow(prediction_decoded[i] - target_decoded[i], 2);
	}
	return error / (double)prediction_decoded.size();
}

double NeuralNetwork::relative_squared_error(std::vector<float>& prediction, std::vector<float>& target, const IEncoder* encoder) {
	auto prediction_decoded = encoder->decode(prediction);
	auto target_decoded = encoder->decode(target);
	double error = 0.0;
	for (size_t i = 0; i < prediction_decoded.size(); i++) {
		auto target_i = target_decoded[i];
		auto prediction_i = prediction_decoded[i];
		auto diff = target_i - prediction_i;
		error += diff * diff / std::max(1.0F, std::max(target_i * target_i, prediction_i * prediction_i));
	}
	return error / (double)prediction_decoded.size();
}

double NeuralNetwork::absolute_error(std::vector<float>& prediction, std::vector<float>& target, const IEncoder* encoder) {
	auto prediction_decoded = encoder->decode(prediction);
	auto target_decoded = encoder->decode(target);
	double error = 0.0;
	for (size_t i = 0; i < prediction_decoded.size(); i++) {
		error += std::abs(prediction_decoded[i] - target_decoded[i]);
	}
	return error / (double)prediction_decoded.size();
}

double NeuralNetwork::relative_absolute_error(std::vector<float>& prediction, std::vector<float>& target, const IEncoder* encoder) {
	auto prediction_decoded = encoder->decode(prediction);
	auto target_decoded = encoder->decode(target);
	double error = 0.0;
	for (size_t i = 0; i < prediction_decoded.size(); i++) {
		auto target_i = target_decoded[i];
		auto prediction_i = prediction_decoded[i];
		auto diff = std::abs(target_i - prediction_i);
		error += diff / std::max(1.0F, std::max(std::abs(target_i), std::abs(prediction_i)));
	}
	return error / (double)prediction_decoded.size();
}

double NeuralNetwork::get_loss_mrse(const Eigen::MatrixXf& inputs, const Eigen::MatrixXf& targets, const IEncoder* encoder) {
	double error = 0.0;
	for (size_t i = 0; i < inputs.rows(); i++) {
		auto input_vector = get_row(inputs, i);
		auto target_vector = get_row(targets, i);
		auto prediction = this->predict(input_vector);
		error += relative_squared_error(prediction, target_vector, encoder);
	}
	return error / (double)inputs.rows();
}

double NeuralNetwork::get_loss_mse(const Eigen::MatrixXf& inputs, const Eigen::MatrixXf& targets, const IEncoder* encoder) {
	double error = 0.0;
	for (size_t i = 0; i < inputs.rows(); i++) {
		auto input_vector = get_row(inputs, i);
		auto target_vector = get_row(targets, i);
		auto prediction = this->predict(input_vector);
		error += squared_error(prediction, target_vector, encoder);
	}
	return error / (double)inputs.rows();
}

double NeuralNetwork::get_acc_mrae(const Eigen::MatrixXf& inputs, const Eigen::MatrixXf& targets, const IEncoder* encoder) {
	double error = 0.0;
	for (size_t i = 0; i < inputs.rows(); i++) {
		auto input_vector = get_row(inputs, i);
		auto target_vector = get_row(targets, i);
		auto prediction = this->predict(input_vector);
		error += relative_absolute_error(prediction, target_vector, encoder);
	}
	return 1.0 - (error / (double)inputs.rows());
}

double NeuralNetwork::get_acc_mae(const Eigen::MatrixXf& inputs, const Eigen::MatrixXf& targets, const IEncoder* encoder) {
	double error = 0.0;
	for (size_t i = 0; i < inputs.rows(); i++) {
		auto input_vector = get_row(inputs, i);
		auto target_vector = get_row(targets, i);
		auto prediction = this->predict(input_vector);
		error += absolute_error(prediction, target_vector, encoder);
	}
	return 1.0 - (error / (double)inputs.rows());
}

void NeuralNetwork::reset() {
	for (size_t i = 0; i < m_weights.size(); i++) {
		for (size_t j = 0; j < m_weights[i].rows(); j++) {
			for (size_t k = 0; k < m_weights[i].cols(); k++) {
				m_weights[i](j, k) = 0.02F * (static_cast<float>(rand()) / RAND_MAX) - 0.01F;
			}
		}
		for (size_t j = 0; j < m_biases[i].cols(); j++) {
			m_biases[i](0, j) = 0.0F;
		}
	}
}

void NeuralNetwork::log_epoch_metrics(size_t epoch, const Eigen::MatrixXf& train_inputs, const Eigen::MatrixXf& train_targets,
									  const Eigen::MatrixXf& validation_inputs, const Eigen::MatrixXf& validation_targets, Dataset& dataset,
									  std::ofstream& log_file, const std::string& file_name) {

	// Calculate the loss and accuracy of the training and validation sets
	double train_loss_mrse = this->get_loss_mrse(train_inputs, train_targets, dataset.get_output_encoder());
	double train_loss_mse = this->get_loss_mse(train_inputs, train_targets, dataset.get_output_encoder());
	double train_acc_mrae = this->get_acc_mrae(train_inputs, train_targets, dataset.get_output_encoder());
	double train_acc_mae = this->get_acc_mae(train_inputs, train_targets, dataset.get_output_encoder());

	// Calculate the loss and accuracy of the validation set if it exists
	double validation_loss_mrse = NAN;
	double validation_loss_mse = NAN;
	double validation_acc_mae = NAN;
	double validation_acc_mrae = NAN;
	if (validation_inputs.rows() > 0) {
		validation_loss_mrse = this->get_loss_mrse(validation_inputs, validation_targets, dataset.get_output_encoder());
		validation_loss_mse = this->get_loss_mse(validation_inputs, validation_targets, dataset.get_output_encoder());
		validation_acc_mae = this->get_acc_mae(validation_inputs, validation_targets, dataset.get_output_encoder());
		validation_acc_mrae = this->get_acc_mrae(validation_inputs, validation_targets, dataset.get_output_encoder());
	}

	// Create log stream
	std::stringstream stream;
	stream << "Epoch: " << epoch << "\t,";
	stream << "Training loss (MRSE): " << train_loss_mrse << "\t,";
	stream << "Validation loss (MRSE): " << validation_loss_mrse << "\t,";
	stream << "Training loss (MSE): " << train_loss_mse << "\t,";
	stream << "Validation loss (MSE): " << validation_loss_mse << "\t,";
	stream << "Training accuracy (MAE): " << train_acc_mae << "\t,";
	stream << "Validation accuracy (MAE): " << validation_acc_mae << "\t,";
	stream << "Training accuracy (MRAE): " << train_acc_mrae << "\t,";
	stream << "Validation accuracy (MRAE): " << validation_acc_mrae << '\n';

	// Print and log
	std::cout << "Epoch: " << epoch << ", Accuracy (MRAE): " << train_acc_mrae << " for " << file_name << '\n';
	log_file << stream.str();
}

void NeuralNetwork::log_final_results(std::ofstream& log_file, Dataset& dataset, const Eigen::MatrixXf& train_inputs,
									  const Eigen::MatrixXf& train_targets, const std::vector<std::string>& train_names,
									  const Eigen::MatrixXf& validation_inputs, const Eigen::MatrixXf& validation_targets,
									  const std::vector<std::string>& validation_names, size_t train_size, size_t validation_size,
									  std::chrono::duration<double> total_time, IOptimiser& optimiser) {

	// Log training time
	log_file << "Training time: " << total_time.count() << "s\n\n";

	log_file << "Final results:\n";

	// Log training data predictions
	log_file << "Training data:\n";
	for (size_t j = 0; j < train_size; j++) {
		auto prediction = this->predict(get_row(train_inputs, j));
		auto prediction_decoded = dataset.get_output_encoder()->decode(prediction);
		auto target = get_row(train_targets, j);
		auto target_decoded = dataset.get_output_encoder()->decode(target);
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

	// Log validation data predictions
	log_file << "\nValidation data:\n";
	for (size_t j = 0; j < validation_size; j++) {
		auto prediction = this->predict(get_row(validation_inputs, j));
		auto prediction_decoded = dataset.get_output_encoder()->decode(prediction);
		auto target = get_row(validation_targets, j);
		auto target_decoded = dataset.get_output_encoder()->decode(target);
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

	// Log network topology
	log_file << "\nNetwork topology:\n";
	for (auto val : this->m_topology) {
		log_file << val << ' ';
	}
	log_file << '\n';

	// Log Optimiser
	log_file << "Optimiser: ";
	char* optimiser_name = abi::__cxa_demangle(typeid(optimiser).name(), 0, 0, nullptr);
	log_file << optimiser_name << '\n';
	log_file << "Learning rate: " << optimiser.get_learning_rate() << '\n';
	free(optimiser_name);

	// Log activation function
	char* activation_func_name = abi::__cxa_demangle(typeid(*this->m_activation_func).name(), 0, 0, nullptr);
	log_file << "Activation function: " << activation_func_name << '\n';
	free(activation_func_name);
}
