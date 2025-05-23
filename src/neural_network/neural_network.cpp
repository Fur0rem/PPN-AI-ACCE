/**
 * @file src/neural_network/neural_network.cpp
 * @brief Implementation of the NeuralNetwork class
 */

#include "neural_network/neural_network.hpp"
#include "neural_network/activation_functions.hpp"
#include "neural_network/gradients.hpp"
#include "neural_network/optimiser.hpp"
#include "neural_network/sparse_multiplication.hpp"
#include <cmath>
#include <cstdlib>
#include <cxxabi.h>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/src/Core/Matrix.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

std::vector<float> get_row(const Eigen::MatrixXf& matrix, size_t row) {
	std::vector<float> result(matrix.cols());
	// #pragma omp parallel for
	for (size_t i = 0; i < matrix.cols(); i++) {
		result[i] = matrix(row, i);
	}
	return result;
}

TrainingNoise::TrainingNoise(float dropout_rate, float add_noise, float mult_noise, float regularisation_term)
	: m_dropout_rate(dropout_rate), m_add_noise(add_noise), m_mult_noise(mult_noise), m_regularisation_term(regularisation_term) {
	// Create the random device
	m_gen = std::mt19937(m_rd());								// Seed the generator with the random device
	m_dist = std::bernoulli_distribution(1.0 - m_dropout_rate); // Bernoulli distribution for dropout
}

/**
 * @brief Apply the noise to the given matrix
 * @param matrix The matrix to apply the noise to
 * @return The matrix with noise applied
 */
void TrainingNoise::apply_noise(Eigen::MatrixXf& matrix) {
	// Apply drop-out
	if (m_dropout_rate > 0.0f) {
		std::random_device rd;									// Random device for seed
		std::mt19937 gen(rd());									// Mersenne Twister random number generator
		std::bernoulli_distribution dist(1.0 - m_dropout_rate); // Bernoulli distribution for dropout

		// #pragma omp parallel for
		for (size_t j = 0; j < matrix.cols(); j++) {
			for (size_t i = 0; i < matrix.rows(); i++) {
				if (dist(gen)) {
					matrix(i, j) = 0.0f;
				}
			}
		}
	}

	// Apply noise
	if (m_add_noise > 0.0F) {
		std::uniform_real_distribution<float> dist(-m_add_noise, m_add_noise);
		// #pragma omp parallel for
		for (size_t j = 0; j < matrix.cols(); j++) {
			for (size_t i = 0; i < matrix.rows(); i++) {
				matrix(i, j) += dist(m_gen);
			}
		}
	}

	if (m_mult_noise > 0.0F) {
		std::uniform_real_distribution<float> dist(1.0F - m_mult_noise, 1.0F + m_mult_noise);
		// #pragma omp parallel for
		for (size_t j = 0; j < matrix.cols(); j++) {
			for (size_t i = 0; i < matrix.rows(); i++) {
				matrix(i, j) *= dist(m_gen);
			}
		}
	}
}

float TrainingNoise::get_regularisation_term() const {
	return m_regularisation_term;
}

NeuralNetwork::NeuralNetwork(const std::vector<size_t>& topology, std::unique_ptr<ActivationFunc> activation_func,
							 std::unique_ptr<TrainingNoise> training_noise)
	: m_topology(topology), m_activation_func(std::move(activation_func)), m_training_noise(std::move(training_noise)) {
	for (size_t i = 0; i < topology.size() - 1; i++) {
		// Initialize weights with small random values
		m_weights.push_back(Eigen::MatrixXf(topology[i], topology[i + 1]));
		// #pragma omp parallel for
		for (size_t k = 0; k < topology[i + 1]; k++) {
			for (size_t j = 0; j < topology[i]; j++) {
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
	// #pragma omp parallel for
	for (size_t i = 0; i < input.size(); i++) {
		input_matrix(0, i) = input[i];
	}

	// Don't add noise to the input
	std::unique_ptr<TrainingNoise> temp_noise = std::move(m_training_noise);
	m_training_noise = nullptr; // Disable noise for prediction
	Eigen::MatrixXf output = this->feed_forward(input_matrix);
	m_training_noise = std::move(temp_noise); // Re-enable noise

	std::vector<float> result(output.cols());
	// #pragma omp parallel for
	for (size_t i = 0; i < output.cols(); i++) {
		result[i] = output(0, i);
	}
	return result;
}

Eigen::MatrixXf NeuralNetwork::predict(const Eigen::MatrixXf& input) {
	// Don't add noise to the input
	std::unique_ptr<TrainingNoise> temp_noise = std::move(m_training_noise);
	m_training_noise = nullptr; // Disable noise for prediction
	Eigen::MatrixXf output = this->feed_forward(input);
	m_training_noise = std::move(temp_noise); // Re-enable noise
	return output;
}

Eigen::MatrixXf NeuralNetwork::feed_forward(const Eigen::MatrixXf& input) {
	m_a_values.clear();
	m_z_values.clear();
	m_a_values.push_back(input);

	for (size_t i = 0; i < m_topology.size() - 1; i++) {
		// Pass the input through the layer
		Eigen::MatrixXf last_a = m_a_values.back();
		Eigen::MatrixXf z;

		if (i == 0) {
			// Use sparse multiplication for the first layer, since it is sparse
			z = eigen_sparse_multiplication(last_a, m_weights[i]);
		}
		else {
			// Use dense multiplication for other layers
			z = last_a * m_weights[i];
		}

		// Add biases
		// #pragma omp parallel for
		for (size_t k = 0; k < z.cols(); k++) {
			for (size_t j = 0; j < z.rows(); j++) {
				z(j, k) += m_biases[i](0, k);
			}
		}
		m_z_values.push_back(z);

		// Make it go through the activation function
		Eigen::MatrixXf a(z.rows(), z.cols());
		// #pragma omp parallel for
		for (size_t k = 0; k < z.cols(); k++) {
			for (size_t j = 0; j < z.rows(); j++) {
				a(j, k) = m_activation_func->func(z(j, k));
			}
		}

		// Noise the matrix
		if (m_training_noise) {
			// Don't noise the last layer
			if (i < m_topology.size() - 2) {
				m_training_noise->apply_noise(a);
			}
		}

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
		// Eigen::MatrixXf derivative = m_z_values[i].unaryExpr([this](float z) {
		// 	return m_activation_func->deriv(z);
		// });
		Eigen::MatrixXf derivative(m_z_values[i].rows(), m_z_values[i].cols());
		for (size_t k = 0; k < m_z_values[i].cols(); k++) {
			for (size_t j = 0; j < m_z_values[i].rows(); j++) {
				derivative(j, k) = m_activation_func->deriv(m_z_values[i](j, k));
			}
		}

		// Compute the gradients
		Eigen::MatrixXf dz = loss_grad.cwiseProduct(derivative);
		Eigen::MatrixXf dw = m_a_values[i].transpose() * dz;
		dw /= (float)m;
		Eigen::MatrixXf db(1, dz.cols());
		// #pragma omp parallel for
		for (size_t k = 0; k < dz.cols(); k++) {
			db(0, k) = dz.col(k).sum() / (float)m;
		}
		loss_grad = dz * m_weights[i].transpose();
		grads.d_w().insert(grads.d_w().begin(), dw);
		grads.d_b().insert(grads.d_b().begin(), db);
	}
	return grads;
}

struct Data {
	std::vector<std::string> train_names;
	std::vector<std::string> validation_names;
	Eigen::MatrixXf train_input_matrix;
	Eigen::MatrixXf train_target_matrix;
	Eigen::MatrixXf validation_input_matrix;
	Eigen::MatrixXf validation_target_matrix;
	size_t train_size;
	size_t validation_size;
};

Data into_data(Dataset& dataset, float training_proportion) {
	auto data = dataset.get_data(1.0);
	// Split the data into training and validation sets
	size_t const train_size = (size_t)((double)data.size() * training_proportion);
	size_t const validation_size = data.size() - train_size;

	// std::cout << "Training size: " << train_size << ", Validation size: " << validation_size << '\n';

	std::vector<Eigen::VectorXf*> train_input_vectors(train_size);
	std::vector<Eigen::VectorXf*> validation_input_vectors(validation_size);
	std::vector<Eigen::VectorXf*> train_target_vectors(train_size);
	std::vector<Eigen::VectorXf*> validation_target_vectors(validation_size);
	std::vector<std::string> train_names(train_size);
	std::vector<std::string> validation_names(validation_size);

	// #pragma omp parallel for
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
	Eigen::MatrixXf validation_input_matrix(validation_size, train_input_vectors[0]->size());
	Eigen::MatrixXf validation_target_matrix(validation_size, matrix_y_for_validation);

	// #pragma omp parallel for
	for (size_t j = 0; j < train_input_vectors[0]->size(); j++) {
		for (size_t i = 0; i < train_size; i++) {
			train_input_matrix(i, j) = (*train_input_vectors[i])(j);
		}
		for (size_t i = 0; i < validation_size; i++) {
			validation_input_matrix(i, j) = (*validation_input_vectors[i])(j);
		}
	}
	// #pragma omp parallel for
	for (size_t j = 0; j < train_target_vectors[0]->size(); j++) {
		for (size_t i = 0; i < train_size; i++) {
			train_target_matrix(i, j) = (*train_target_vectors[i])(j);
		}
		for (size_t i = 0; i < validation_size; i++) {
			validation_target_matrix(i, j) = (*validation_target_vectors[i])(j);
		}
	}

	Data data_struct = {
		.train_names = train_names,
		.validation_names = validation_names,
		.train_input_matrix = train_input_matrix,
		.train_target_matrix = train_target_matrix,
		.validation_input_matrix = validation_input_matrix,
		.validation_target_matrix = validation_target_matrix,
		.train_size = train_size,
		.validation_size = validation_size,
	};
	return data_struct;
}

std::pair<float, float> NeuralNetwork::train(Dataset& dataset, size_t nb_epochs, float training_proportion, float learning_rate,
											 std::string&& logging_dir, size_t nb_trains) {

	// Create the logging directory
	std::filesystem::create_directory(logging_dir);

	// Get the data
	// Inline the into_data function
	auto data = dataset.get_data(1.0);

	if (data.empty()) {
		throw std::runtime_error("Dataset is empty.");
	}

	size_t const train_size = (size_t)((double)data.size() * training_proportion);
	size_t const validation_size = data.size() - train_size;

	std::cout << "Training size: " << train_size << ", Validation size: " << validation_size << '\n';

	std::vector<Eigen::VectorXf*> train_input_vectors(train_size);
	std::vector<Eigen::VectorXf*> validation_input_vectors(validation_size);
	std::vector<Eigen::VectorXf*> train_target_vectors(train_size);
	std::vector<Eigen::VectorXf*> validation_target_vectors(validation_size);
	std::vector<std::string> train_names(train_size);
	std::vector<std::string> validation_names(validation_size);

	// #pragma omp parallel for
	for (size_t i = 0; i < data.size(); i++) {
		if (std::get<1>(data[i]).size() == 0 || std::get<2>(data[i]).size() == 0) {
			throw std::runtime_error("Invalid data point: Input or target vector is empty.");
		}

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

	size_t matrix_y_for_validation = (validation_size > 0) ? validation_target_vectors[0]->size() : 0;
	Eigen::MatrixXf validation_input_matrix(validation_size, train_input_vectors[0]->size());
	Eigen::MatrixXf validation_target_matrix(validation_size, matrix_y_for_validation);

	// #pragma omp parallel for
	for (size_t j = 0; j < train_input_vectors[0]->size(); j++) {
		for (size_t i = 0; i < train_size; i++) {
			train_input_matrix(i, j) = (*train_input_vectors[i])(j);
		}
		for (size_t i = 0; i < validation_size; i++) {
			validation_input_matrix(i, j) = (*validation_input_vectors[i])(j);
		}
	}
	// #pragma omp parallel for
	for (size_t j = 0; j < train_target_vectors[0]->size(); j++) {
		for (size_t i = 0; i < train_size; i++) {
			train_target_matrix(i, j) = (*train_target_vectors[i])(j);
		}
		for (size_t i = 0; i < validation_size; i++) {
			validation_target_matrix(i, j) = (*validation_target_vectors[i])(j);
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
				// #pragma omp parallel for
				for (size_t k = 0; k < input->size(); k++) {
					input_matrix(0, k) = (*input)(k);
				}
				Eigen::MatrixXf target_matrix(1, target->size());
				// #pragma omp parallel for
				for (size_t k = 0; k < target->size(); k++) {
					target_matrix(0, k) = (*target)(k);
				}
				// Eigen::MatrixXf output = this->feed_forward(input_matrix);
				Eigen::MatrixXf output = this->feed_forward(input_matrix);
				// Backward pass
				Gradients grads = this->backward(input_matrix, target_matrix);
				// Update weights and biases
				Eigen::MatrixXf errors(1, target->size());
				// #pragma omp parallel for
				for (size_t k = 0; k < target->size(); k++) {
					errors(0, k) = (*target)(k)-output(0, k);
				}
				// Update weights and biases
				// #pragma omp parallel for
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
												   IOptimiser& optimiser, std::string&& logging_dir, size_t nb_trains) {

	// Create the logging directory
	std::filesystem::create_directory(logging_dir);

	// Get the data
	Data data = into_data(dataset, training_proportion);
	auto& train_names = data.train_names;
	auto& validation_names = data.validation_names;
	auto& train_input_matrix = data.train_input_matrix;
	auto& train_target_matrix = data.train_target_matrix;
	auto& validation_input_matrix = data.validation_input_matrix;
	auto& validation_target_matrix = data.validation_target_matrix;
	size_t train_size = data.train_size;
	size_t validation_size = data.validation_size;

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

			std::vector<size_t> indices(train_input_matrix.rows());
			for (size_t i = 0; i < train_input_matrix.rows(); i++) {
				indices[i] = i;
			}
			std::shuffle(indices.begin(), indices.end(), std::mt19937(std::random_device()()));

			Eigen::MatrixXf x_shuffled(train_input_matrix.rows(), train_input_matrix.cols());
			Eigen::MatrixXf y_shuffled(train_target_matrix.rows(), train_target_matrix.cols());
			// #pragma omp parallel for
			for (size_t j = 0; j < train_input_matrix.cols(); j++) {
				for (size_t i = 0; i < train_input_matrix.rows(); i++) {
					x_shuffled(i, j) = train_input_matrix(indices[i], j);
				}
			}
			// #pragma omp parallel for
			for (size_t j = 0; j < train_target_matrix.cols(); j++) {
				for (size_t i = 0; i < train_target_matrix.rows(); i++) {
					y_shuffled(i, j) = train_target_matrix(indices[i], j);
				}
			}

			// Mini-batch gradient descent
			for (size_t i = 0; i < train_input_matrix.rows(); i += batch_size) {
				size_t end = std::min(i + batch_size, (size_t)train_input_matrix.rows());

				// Get the batch
				auto x_batch = x_shuffled.block(i, 0, end - i, train_input_matrix.cols());
				auto y_batch = y_shuffled.block(i, 0, end - i, train_target_matrix.cols());

				// Forward pass
				this->feed_forward(x_batch);

				// Backward pass
				Gradients grads = this->backward(x_batch, y_batch);

				auto regularisation_term = this->m_training_noise->get_regularisation_term();

				// Add regularisation term (L2 regularisation), penalises large weights
				if (regularisation_term > 0.0) {
					// #pragma omp parallel for
					for (size_t i = 0; i < m_topology.size() - 1; i++) {
						grads.d_w()[i] += regularisation_term * m_weights[i].cwiseProduct(m_weights[i]);
					}
				}

				// Update weights
				optimiser.update_weights(grads, *this);
			}

			auto end_clock = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed_seconds = end_clock - clock;
			total_time += elapsed_seconds;

			const int nb_points_to_plot = (nb_epochs > 1000) ? nb_epochs / 1000 : 1;
			if (epoch % nb_points_to_plot == 0) {
				this->log_epoch_metrics(epoch,
										train_input_matrix,
										train_target_matrix,
										validation_input_matrix,
										validation_target_matrix,
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
								train_input_matrix,
								train_target_matrix,
								train_names,
								validation_input_matrix,
								validation_target_matrix,
								validation_names,
								train_size,
								validation_size,
								total_time,
								optimiser);

		log_file.close();
	}

	// Evaluate the model (accuracy of the training and validation sets)
	double train_acc = get_acc_mrae(train_input_matrix, train_target_matrix, dataset.get_output_encoder());
	double validation_acc = get_acc_mrae(validation_input_matrix, validation_target_matrix, dataset.get_output_encoder());
	return std::make_pair(train_acc, validation_acc);
}

std::pair<float, float> NeuralNetwork::train_batch_for_topology_finder(Dataset& dataset, size_t nb_epochs, float training_proportion,
																	   size_t batch_size, IOptimiser& optimiser,
																	   float train_acc_threshold_at_half,
																	   float validation_acc_threshold_at_half) {
	// Get the data
	Data data = into_data(dataset, training_proportion);
	auto& train_input_matrix = data.train_input_matrix;
	auto& train_target_matrix = data.train_target_matrix;
	auto& validation_input_matrix = data.validation_input_matrix;
	auto& validation_target_matrix = data.validation_target_matrix;
	size_t train_size = data.train_size;
	size_t validation_size = data.validation_size;

	// Reset the neural network to avoid using the previous weights
	this->reset();

	// Training the neural network
	for (size_t i = 0; i < nb_epochs; i++) {
		// Shuffle data
		std::vector<size_t> indices(train_input_matrix.rows());
		for (size_t i = 0; i < train_input_matrix.rows(); i++) {
			indices[i] = i;
		}
		std::shuffle(indices.begin(), indices.end(), std::mt19937(std::random_device()()));
		Eigen::MatrixXf x_shuffled(train_input_matrix.rows(), train_input_matrix.cols());
		Eigen::MatrixXf y_shuffled(train_target_matrix.rows(), train_target_matrix.cols());
		// #pragma omp parallel for
		for (size_t j = 0; j < train_input_matrix.cols(); j++) {
			for (size_t i = 0; i < train_input_matrix.rows(); i++) {
				x_shuffled(i, j) = train_input_matrix(indices[i], j);
			}
		}
		// #pragma omp parallel for
		for (size_t j = 0; j < train_target_matrix.cols(); j++) {
			for (size_t i = 0; i < train_target_matrix.rows(); i++) {
				y_shuffled(i, j) = train_target_matrix(indices[i], j);
			}
		}

		// Mini-batch gradient descent
		for (size_t i = 0; i < train_input_matrix.rows(); i += batch_size) {
			size_t end = std::min(i + batch_size, (size_t)train_input_matrix.rows());

			// Get the batch
			auto x_batch = x_shuffled.block(i, 0, end - i, train_input_matrix.cols());
			auto y_batch = y_shuffled.block(i, 0, end - i, train_target_matrix.cols());

			// Feed it into the neural network
			this->feed_forward(x_batch);
			Gradients grads = this->backward(x_batch, y_batch);
			optimiser.update_weights(grads, *this);
		}

		// If you're at half the epochs, check if the accuracy is good enough
		if (i == nb_epochs / 2) {
			double train_acc = get_acc_mrae(train_input_matrix, train_target_matrix, dataset.get_output_encoder());
			double validation_acc = get_acc_mrae(validation_input_matrix, validation_target_matrix, dataset.get_output_encoder());
			if (train_acc < train_acc_threshold_at_half || validation_acc < validation_acc_threshold_at_half) {
				return std::make_pair(train_acc, validation_acc);
			}
		}
	}

	// Evaluate the model (accuracy of the training and validation sets)
	double train_acc = get_acc_mrae(train_input_matrix, train_target_matrix, dataset.get_output_encoder());
	double validation_acc = get_acc_mrae(validation_input_matrix, validation_target_matrix, dataset.get_output_encoder());
	return std::make_pair(train_acc, validation_acc);
}

std::pair<float, float> NeuralNetwork::train_local_search(Dataset& dataset, size_t nb_epochs, float training_proportion, size_t nb_samples,
														  std::string&& logging_dir, size_t nb_trains) {
	// Create the logging directory
	std::filesystem::create_directory(logging_dir);

	// Get the data
	Data data = into_data(dataset, training_proportion);
	auto& train_names = data.train_names;
	auto& validation_names = data.validation_names;
	auto& train_input_matrix = data.train_input_matrix;
	auto& train_target_matrix = data.train_target_matrix;
	auto& validation_input_matrix = data.validation_input_matrix;
	auto& validation_target_matrix = data.validation_target_matrix;
	size_t train_size = data.train_size;
	size_t validation_size = data.validation_size;

	for (size_t train = 0; train < nb_trains; train++) {
		std::chrono::duration<double> total_time = std::chrono::duration<double>::zero();
		// Reset the neural network to avoid using the previous weights
		this->reset();

		// Open the file for logging
		std::string logging_filename = logging_dir + "/train_" + std::to_string(train) + ".log";
		std::ofstream log_file;
		log_file.open(logging_filename);

		for (size_t epoch = 0; epoch < nb_epochs; epoch++) {
			auto clock = std::chrono::high_resolution_clock::now();

			// Backup the original weights and biases
			std::vector<Eigen::MatrixXf> original_weights = m_weights;
			std::vector<Eigen::MatrixXf> original_biases = m_biases;
			std::vector<Eigen::MatrixXf> best_weights = m_weights;
			std::vector<Eigen::MatrixXf> best_biases = m_biases;

			double best_train_acc = 0.0;

			// For each sample, generate a random perturbation, and keep the best one
			for (size_t i = 0; i < nb_samples; i++) {
				// Generate a random perturbation
				m_weights = original_weights;
				m_biases = original_biases;

				for (size_t j = 0; j < m_weights.size(); j++) {
					// for (size_t k = 0; k < m_weights[j].rows(); k++) {
					// 	for (size_t l = 0; l < m_weights[j].cols(); l++) {
					// 		m_weights[j](k, l) += ((float)rand() / RAND_MAX - 0.5F) * 0.03F;
					// 	}
					// }
					for (size_t l = 0; l < m_weights[j].cols(); l++) {
						for (size_t k = 0; k < m_weights[j].rows(); k++) {
							m_weights[j](k, l) += ((float)rand() / RAND_MAX - 0.5F) * 0.03F;
						}
					}
					for (size_t k = 0; k < m_biases[j].cols(); k++) {
						m_biases[j](0, k) += ((float)rand() / RAND_MAX - 0.5F) * 0.03F;
					}
				}

				// Evaluate the model
				double train_acc = this->get_acc_mrae(train_input_matrix, train_target_matrix, dataset.get_output_encoder());
				if (train_acc > best_train_acc) {
					best_train_acc = train_acc;
					best_weights = m_weights;
					best_biases = m_biases;
				}

				// Restore the original weights and biases
				m_weights = original_weights;
				m_biases = original_biases;
			}

			// Update the weights and biases with the best ones
			m_weights = best_weights;
			m_biases = best_biases;
			auto end_clock = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed_seconds = end_clock - clock;
			total_time += elapsed_seconds;

			const int nb_points_to_plot = (nb_epochs > 1000) ? nb_epochs / 1000 : 1;
			if (epoch % nb_points_to_plot == 0) {
				this->log_epoch_metrics(epoch,
										train_input_matrix,
										train_target_matrix,
										validation_input_matrix,
										validation_target_matrix,
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
		IOptimiser* opt = new SGD(0.00F);
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

std::pair<float, float> NeuralNetwork::train_simulated_annealing(Dataset& dataset, size_t nb_epochs, float training_proportion,
																 float decay_rate, size_t nb_tracked, float initial_temp,
																 std::string&& logging_dir, size_t nb_trains) {
	// Create the logging directory
	std::filesystem::create_directory(logging_dir);

	// Get the data
	Data data = into_data(dataset, training_proportion);
	auto& train_names = data.train_names;
	auto& validation_names = data.validation_names;
	auto& train_input_matrix = data.train_input_matrix;
	auto& train_target_matrix = data.train_target_matrix;
	auto& validation_input_matrix = data.validation_input_matrix;
	auto& validation_target_matrix = data.validation_target_matrix;
	size_t train_size = data.train_size;
	size_t validation_size = data.validation_size;

	for (size_t train = 0; train < nb_trains; train++) {
		std::chrono::duration<double> total_time = std::chrono::duration<double>::zero();
		// Reset the neural network to avoid using the previous weights
		this->reset();

		// Open the file for logging
		std::string logging_filename = logging_dir + "/train_" + std::to_string(train) + ".log";
		std::ofstream log_file;
		log_file.open(logging_filename);

		// acc, weights, biases
		std::vector<std::pair<double, std::pair<std::vector<Eigen::MatrixXf>, std::vector<Eigen::MatrixXf>>>> tracked_candidates(
			nb_tracked);

		for (size_t epoch = 0; epoch < nb_epochs; epoch++) {
			auto clock = std::chrono::high_resolution_clock::now();

			// Backup the original weights and biases
			std::vector<Eigen::MatrixXf> original_weights = m_weights;
			std::vector<Eigen::MatrixXf> original_biases = m_biases;
			std::vector<Eigen::MatrixXf> best_weights = m_weights;
			std::vector<Eigen::MatrixXf> best_biases = m_biases;

			double current_temp = initial_temp * std::pow(decay_rate, epoch);
			if (current_temp < 0.001F) {
				current_temp = 0.001F;
			}
			std::cout << "Current temperature: " << current_temp << '\n';

			// For each candidate, generate a random perturbation, and insert it into the tracked candidates list if it's better
			auto current_candidates = tracked_candidates;

			for (size_t i = 0; i < nb_tracked; i++) {
				// Generate a random perturbation
				m_weights = original_weights;
				m_biases = original_biases;

				for (size_t j = 0; j < m_weights.size(); j++) {
					for (size_t k = 0; k < m_weights[j].rows(); k++) {
						for (size_t l = 0; l < m_weights[j].cols(); l++) {
							m_weights[j](k, l) += ((float)rand() / RAND_MAX - 0.5F) * current_temp;
						}
					}
					for (size_t k = 0; k < m_biases[j].cols(); k++) {
						m_biases[j](0, k) += ((float)rand() / RAND_MAX - 0.5F) * current_temp;
					}
				}
				// Evaluate the model
				double train_acc = this->get_acc_mrae(train_input_matrix, train_target_matrix, dataset.get_output_encoder());

				// Insert it into the tracked candidates if better
				std::sort(tracked_candidates.begin(), tracked_candidates.end(), [](const auto& a, const auto& b) {
					return a.first < b.first;
				});
				if (train_acc > tracked_candidates[0].first) {
					tracked_candidates[0].first = train_acc;
					tracked_candidates[0].second.first = m_weights;
					tracked_candidates[0].second.second = m_biases;
				}
				// std::cout << "Accuracy: " << train_acc << '\n';
			}

			auto end_clock = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed_seconds = end_clock - clock;
			total_time += elapsed_seconds;

			// Use best candidate for logging
			std::sort(tracked_candidates.begin(), tracked_candidates.end(), [](const auto& a, const auto& b) {
				return a.first > b.first;
			});
			m_weights = tracked_candidates[0].second.first;
			m_biases = tracked_candidates[0].second.second;

			const int nb_points_to_plot = (nb_epochs > 1000) ? nb_epochs / 1000 : 1;
			if (epoch % nb_points_to_plot == 0) {
				this->log_epoch_metrics(epoch,
										train_input_matrix,
										train_target_matrix,
										validation_input_matrix,
										validation_target_matrix,
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
		IOptimiser* opt = new SGD(0.00F);
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

std::pair<float, float> NeuralNetwork::train_simulated_annealing_and_gradient(Dataset& dataset, size_t nb_epochs, float training_proportion,
																			  float decay_rate, size_t nb_tracked, float initial_temp,
																			  IOptimiser& optimiser, size_t nb_epochs_gradient,
																			  size_t batch_size_gradient, std::string&& logging_dir,
																			  size_t nb_trains) {
	// Create the logging directory
	std::filesystem::create_directory(logging_dir);

	// Get the data
	Data data = into_data(dataset, training_proportion);
	auto& train_names = data.train_names;
	auto& validation_names = data.validation_names;
	auto& train_input_matrix = data.train_input_matrix;
	auto& train_target_matrix = data.train_target_matrix;
	auto& validation_input_matrix = data.validation_input_matrix;
	auto& validation_target_matrix = data.validation_target_matrix;
	size_t train_size = data.train_size;
	size_t validation_size = data.validation_size;

	for (size_t train = 0; train < nb_trains; train++) {
		std::chrono::duration<double> total_time = std::chrono::duration<double>::zero();
		// Reset the neural network to avoid using the previous weights
		this->reset();

		// Open the file for logging
		std::string logging_filename = logging_dir + "/train_" + std::to_string(train) + ".log";
		std::ofstream log_file;
		log_file.open(logging_filename);

		// acc, weights, biases
		std::vector<std::pair<double, std::pair<std::vector<Eigen::MatrixXf>, std::vector<Eigen::MatrixXf>>>> tracked_candidates(
			nb_tracked);

		const int nb_points_to_plot = (nb_epochs > 1000) ? nb_epochs / 1000 : 1;

		size_t current_total_epochs = 0;

		for (size_t epoch = 0; epoch < nb_epochs; epoch++) {
			auto clock = std::chrono::high_resolution_clock::now();

			// Backup the original weights and biases
			std::vector<Eigen::MatrixXf> original_weights = m_weights;
			std::vector<Eigen::MatrixXf> original_biases = m_biases;
			std::vector<Eigen::MatrixXf> best_weights = m_weights;
			std::vector<Eigen::MatrixXf> best_biases = m_biases;

			double current_temp = initial_temp * std::pow(decay_rate, epoch);
			if (current_temp < 0.001F) {
				current_temp = 0.001F;
			}
			std::cout << "Current temperature: " << current_temp << '\n';

			// For each candidate, generate a random perturbation, and insert it into the tracked candidates list if it's better
			auto current_candidates = tracked_candidates;

			for (size_t i = 0; i < nb_tracked; i++) {
				// Generate a random perturbation
				m_weights = original_weights;
				m_biases = original_biases;

				for (size_t j = 0; j < m_weights.size(); j++) {
					for (size_t k = 0; k < m_weights[j].rows(); k++) {
						for (size_t l = 0; l < m_weights[j].cols(); l++) {
							m_weights[j](k, l) += ((float)rand() / RAND_MAX - 0.5F) * current_temp;
						}
					}
					for (size_t k = 0; k < m_biases[j].cols(); k++) {
						m_biases[j](0, k) += ((float)rand() / RAND_MAX - 0.5F) * current_temp;
					}
				}

				// Do a few iterations of the gradient descent (batched)
				for (size_t sub_epoch = 0; sub_epoch < nb_epochs_gradient; sub_epoch++) {
					// Shuffle data
					std::vector<size_t> indices(train_input_matrix.rows());
					for (size_t i = 0; i < train_input_matrix.rows(); i++) {
						indices[i] = i;
					}
					std::shuffle(indices.begin(), indices.end(), std::mt19937(std::random_device()()));

					Eigen::MatrixXf x_shuffled(train_input_matrix.rows(), train_input_matrix.cols());
					Eigen::MatrixXf y_shuffled(train_target_matrix.rows(), train_target_matrix.cols());
					for (size_t i = 0; i < train_input_matrix.rows(); i++) {
						for (size_t j = 0; j < train_input_matrix.cols(); j++) {
							x_shuffled(i, j) = train_input_matrix(indices[i], j);
						}
					}
					for (size_t i = 0; i < train_target_matrix.rows(); i++) {
						for (size_t j = 0; j < train_target_matrix.cols(); j++) {
							y_shuffled(i, j) = train_target_matrix(indices[i], j);
						}
					}

					// Mini-batch gradient descent
					for (size_t i = 0; i < train_input_matrix.rows(); i += batch_size_gradient) {
						size_t end = std::min(i + batch_size_gradient, (size_t)train_input_matrix.rows());
						Eigen::MatrixXf x_batch(end - i, train_input_matrix.cols());
						Eigen::MatrixXf y_batch(end - i, train_target_matrix.cols());
						for (size_t j = i; j < end; j++) {
							for (size_t k = 0; k < train_input_matrix.cols(); k++) {
								x_batch(j - i, k) = x_shuffled(j, k);
							}
							for (size_t k = 0; k < train_target_matrix.cols(); k++) {
								y_batch(j - i, k) = y_shuffled(j, k);
							}
						}

						// Forward pass
						this->feed_forward(x_batch);

						// Backward pass
						Gradients grads = this->backward(x_batch, y_batch);

						// Add regularisation term (L2 regularisation), penalises large weights
						auto regularisation_term = this->m_training_noise->get_regularisation_term();
						if (regularisation_term > 0.0) {
							for (size_t i = 0; i < m_topology.size() - 1; i++) {
								grads.d_w()[i] += regularisation_term * m_weights[i].cwiseProduct(m_weights[i]);
							}
						}
						// Update weights
						optimiser.update_weights(grads, *this);
					}

					// Logging the loss and accuracy
					if (current_total_epochs % nb_points_to_plot == 0) {
						this->log_epoch_metrics(current_total_epochs,
												train_input_matrix,
												train_target_matrix,
												validation_input_matrix,
												validation_target_matrix,
												dataset,
												log_file,
												logging_filename);
					}

					current_total_epochs++;
				}

				// Evaluate the model
				double train_acc = this->get_acc_mrae(train_input_matrix, train_target_matrix, dataset.get_output_encoder());

				// Insert it into the tracked candidates if better
				std::sort(tracked_candidates.begin(), tracked_candidates.end(), [](const auto& a, const auto& b) {
					return a.first < b.first;
				});
				if (train_acc > tracked_candidates[0].first) {
					tracked_candidates[0].first = train_acc;
					tracked_candidates[0].second.first = m_weights;
					tracked_candidates[0].second.second = m_biases;
				}
				// std::cout << "Accuracy: " << train_acc << '\n';
			}

			auto end_clock = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed_seconds = end_clock - clock;
			total_time += elapsed_seconds;

			// Use best candidate for logging
			std::sort(tracked_candidates.begin(), tracked_candidates.end(), [](const auto& a, const auto& b) {
				return a.first > b.first;
			});
			m_weights = tracked_candidates[0].second.first;
			m_biases = tracked_candidates[0].second.second;

			epoch = current_total_epochs;

			if (epoch % nb_points_to_plot == 0) {
				this->log_epoch_metrics(current_total_epochs,
										train_input_matrix,
										train_target_matrix,
										validation_input_matrix,
										validation_target_matrix,
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
		IOptimiser* opt = new SGD(0.00F);
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
	// std::cout << "Prediction decoded: " << prediction_decoded[0] << ", prediction: " << prediction[0]
	// 		  << ", target decoded: " << target_decoded[0] << ", target: " << target[0] << '\n';
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
	Eigen::MatrixXf predictions = this->feed_forward(inputs);
	Eigen::MatrixXf targets_decoded = encoder->decode_batch(targets);
	Eigen::MatrixXf predictions_decoded = encoder->decode_batch(predictions);
	double error = 0.0;
	// #pragma omp parallel for reduction(+ : error)
	for (size_t i = 0; i < inputs.rows(); i++) {
		double relative_error = 0.0F;
		for (size_t j = 0; j < targets_decoded.cols(); j++) {
			auto target_i = targets_decoded(i, j);
			auto prediction_i = predictions_decoded(i, j);
			auto diff = std::abs(target_i - prediction_i);
			relative_error += diff / std::max(1.0F, std::max(std::abs(target_i), std::abs(prediction_i)));
		}
		error += relative_error / (double)targets_decoded.cols();
	}
	return 1.0 - (error / (double)inputs.rows());
}

double NeuralNetwork::get_acc_mae(const Eigen::MatrixXf& inputs, const Eigen::MatrixXf& targets, const IEncoder* encoder) {
	double error = 0.0;
	// #pragma omp parallel for reduction(+ : error)
	for (size_t i = 0; i < inputs.rows(); i++) {
		auto input_vector = get_row(inputs, i);
		auto target_vector = get_row(targets, i);
		auto prediction = this->predict(input_vector);
		error += absolute_error(prediction, target_vector, encoder);
	}
	return 1.0 - (error / (double)inputs.rows());
}

void NeuralNetwork::reset() {
	// #pragma omp parallel for
	for (size_t i = 0; i < m_weights.size(); i++) {
		for (size_t k = 0; k < m_weights[i].cols(); k++) {
			for (size_t j = 0; j < m_weights[i].rows(); j++) {
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
	// double train_loss_mrse = this->get_loss_mrse(train_inputs, train_targets, dataset.get_output_encoder());
	// double train_loss_mse = this->get_loss_mse(train_inputs, train_targets, dataset.get_output_encoder());
	// double train_acc_mrae = this->get_acc_mrae(train_inputs, train_targets, dataset.get_output_encoder());
	// double train_acc_mae = this->get_acc_mae(train_inputs, train_targets, dataset.get_output_encoder());

	// // Calculate the loss and accuracy of the validation set if it exists
	// double validation_loss_mrse = NAN;
	// double validation_loss_mse = NAN;
	// double validation_acc_mae = NAN;
	// double validation_acc_mrae = NAN;
	// if (validation_inputs.rows() > 0) {
	// 	validation_loss_mrse = this->get_loss_mrse(validation_inputs, validation_targets, dataset.get_output_encoder());
	// 	validation_loss_mse = this->get_loss_mse(validation_inputs, validation_targets, dataset.get_output_encoder());
	// 	validation_acc_mae = this->get_acc_mae(validation_inputs, validation_targets, dataset.get_output_encoder());
	// 	validation_acc_mrae = this->get_acc_mrae(validation_inputs, validation_targets, dataset.get_output_encoder());
	// }

	// We only measure the acc mrae now so it's useless to calculate the other metrics
	double train_loss_mrse = NAN;
	double train_loss_mse = NAN;
	double train_acc_mrae = this->get_acc_mrae(train_inputs, train_targets, dataset.get_output_encoder());
	double train_acc_mae = NAN;
	double validation_loss_mrse = NAN;
	double validation_loss_mse = NAN;
	double validation_acc_mae = NAN;
	double validation_acc_mrae = NAN;
	if (validation_inputs.rows() > 0) {
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