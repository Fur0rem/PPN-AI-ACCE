/**
 * @file include/neural_network/neural_network.hpp
 * @brief The header file for the neural network
 */

#ifndef NEURAL_NETWORK_HPP
#define NEURAL_NETWORK_HPP

#include "dataset/dataset.hpp"
#include "neural_network/activation_functions.hpp"
#include "neural_network/gradients.hpp"
#include "neural_network/optimiser.hpp"
#include <chrono>
#include <eigen3/Eigen/Dense>
#include <memory>
#include <vector>

/**
 * @brief Neural Network class
 * @details This class implements a feedforward neural network with backpropagation.
 */
class NeuralNetwork {
	friend class IOptimiser;
	friend class Adam;
	friend class SGD;
	friend class Momentum;
	friend class RMSProp;
	friend class AMSGrad;

  private:
	std::vector<size_t> m_topology;					   ///< Topology of the neural network
	std::vector<Eigen::MatrixXf> m_weights;			   ///< Weights of the neural network
	std::vector<Eigen::MatrixXf> m_biases;			   ///< Biases of the neural network
	std::vector<Eigen::MatrixXf> m_a_values;		   ///< Activations of the neural network
	std::vector<Eigen::MatrixXf> m_z_values;		   ///< Pre-activations of the neural network
	std::unique_ptr<ActivationFunc> m_activation_func; ///< Activation function of the network

  public:
	/**
	 * @brief Constructor for NeuralNetwork class
	 * @param topology Topology of the neural network
	 * @param activation_func Activation function of the neural network
	 */
	NeuralNetwork(const std::vector<size_t>& topology, std::unique_ptr<ActivationFunc> activation_func);

	/**
	 * @brief Predict the output of the neural network for a given input
	 * @param input The input to get the prediction for
	 * @return Output vector resulting from the prediction
	 */
	std::vector<float> predict(const std::vector<float>& input);

	/**
	 * @brief Predict the output of the neural network for a given input
	 * @param input The input to get the prediction for
	 * @return Output matrix resulting from the prediction
	 */
	Eigen::MatrixXf predict(const Eigen::MatrixXf& input);

	/**
	 * @brief Feed forward the input through the neural network
	 * @param input The input to feed forward
	 * @return Output matrix resulting from the feed forward
	 */
	// Eigen::MatrixXf feed_forward(const Eigen::MatrixXf& input  );
	Eigen::MatrixXf feed_forward(const Eigen::MatrixXf& input, float dropout_rate);

	/**
	 * @brief Backpropagation algorithm to compute gradients
	 * @param inputs A collection of inputs
	 * @param targets Their associated targets
	 * @return Gradients for weights and biases
	 */
	Gradients backward(const Eigen::MatrixXf& inputs, const Eigen::MatrixXf& targets);

	/**
	 * @brief Train the neural network using the given dataset and return the accuracy of training and validation data (MRAE)
	 * Does backpropagation for each input and updates the weights and biases 1 by 1
	 * @param dataset The dataset to train on
	 * @param nb_epochs Number of epochs to train for
	 * @param training_proportion Proportion of data to use for training (The rest will be used for validation)
	 * @param learning_rate Learning rate for the optimiser
	 * @param logging_dir Directory to log results
	 * @param nb_trains Number of training runs
	 * @param dropout_rate Dropout rate for the neural network
	 * @return A pair of floats containing the training and validation accuracy
	 */
	std::pair<float, float> train(Dataset& dataset, size_t nb_epochs, float training_proportion, float learning_rate,
								  std::string&& logging_dir, size_t nb_trains, float dropout_rate);

	/**
	 * @brief Train the neural network using the given dataset with batch training and return the accuracy of training and validation data
	 * (MRAE) This means it will compute the gradients for a batch of data and then update the weights and biases instead of 1 by 1
	 * @param dataset The dataset to train on
	 * @param nb_epochs Number of epochs to train for
	 * @param training_proportion Proportion of data to use for training (The rest will be used for validation)
	 * @param batch_size Size of the batch for training
	 * @param optimiser Optimiser to use for training
	 * @param logging_dir Directory to log results
	 * @param nb_trains Number of training runs
	 * @param dropout_rate Dropout rate for the neural network
	 * @return A pair of floats containing the training and validation accuracy
	 */
	std::pair<float, float> train_batch(Dataset& dataset, size_t nb_epochs, float training_proportion, size_t batch_size,
										IOptimiser& optimiser, std::string&& logging_dir, size_t nb_trains, float dropout_rate);

	/**
	 * @brief Compute the mean relative squared error between the prediction and target
	 * @param prediction The predicted values
	 * @param target The target values
	 * @param encoder The output encoder to bring the values back to their original scale
	 * @return The mean relative squared error
	 */
	double squared_error(std::vector<float>& prediction, std::vector<float>& target, const IEncoder* encoder);

	/**
	 * @brief Compute the mean relative squared error between the prediction and target
	 * @param prediction The predicted values
	 * @param target The target values
	 * @param encoder The output encoder to bring the values back to their original scale
	 * @return The mean relative squared error
	 */
	double relative_squared_error(std::vector<float>& prediction, std::vector<float>& target, const IEncoder* encoder);

	/**
	 * @brief Compute the mean absolute error between the prediction and target
	 * @param prediction The predicted values
	 * @param target The target values
	 * @param encoder The output encoder to bring the values back to their original scale
	 * @return The mean absolute error
	 */
	double absolute_error(std::vector<float>& prediction, std::vector<float>& target, const IEncoder* encoder);

	/**
	 * @brief Compute the mean relative absolute error between the prediction and target
	 * @param prediction The predicted values
	 * @param target The target values
	 * @param encoder The output encoder to bring the values back to their original scale
	 * @return The mean relative absolute error
	 */
	double relative_absolute_error(std::vector<float>& prediction, std::vector<float>& target, const IEncoder* encoder);

	/**
	 * @brief Get the overall loss of the neural network using mean relative squared error
	 * @param inputs The inputs to the neural network
	 * @param targets The targets to compare against
	 * @param encoder The output encoder to bring the values back to their original scale
	 * @return The mean relative squared error
	 */
	double get_loss_mrse(const Eigen::MatrixXf& inputs, const Eigen::MatrixXf& targets, const IEncoder* encoder);

	/**
	 * @brief Get the overall loss of the neural network using mean squared error
	 * @param inputs The inputs to the neural network
	 * @param targets The targets to compare against
	 * @param encoder The output encoder to bring the values back to their original scale
	 * @return The mean squared error
	 */
	double get_loss_mse(const Eigen::MatrixXf& inputs, const Eigen::MatrixXf& targets, const IEncoder* encoder);

	/**
	 * @brief Get the overall accuracy of the neural network using mean relative absolute error
	 * @param inputs The inputs to the neural network
	 * @param targets The targets to compare against
	 * @param encoder The output encoder to bring the values back to their original scale
	 * @return The mean relative absolute error
	 */
	double get_acc_mrae(const Eigen::MatrixXf& inputs, const Eigen::MatrixXf& targets, const IEncoder* encoder);

	/**
	 * @brief Get the overall accuracy of the neural network using mean absolute error
	 * @param inputs The inputs to the neural network
	 * @param targets The targets to compare against
	 * @param encoder The output encoder to bring the values back to their original scale
	 * @return The mean absolute error
	 */
	double get_acc_mae(const Eigen::MatrixXf& inputs, const Eigen::MatrixXf& targets, const IEncoder* encoder);

	/**
	 * @brief Reset the neural network to its initial state
	 * @details This function resets the weights and biases of the neural network to their initial values.
	 */
	void reset();

	/**
	 * @brief Log the results of the training
	 * @param log_file The file to log the results to
	 * @param dataset The dataset used for training
	 * @param train_inputs The inputs used for training
	 * @param train_targets The targets used for training
	 * @param train_names The names of the inputs used for training
	 * @param validation_inputs The inputs used for validation
	 * @param validation_targets The targets used for validation
	 * @param validation_names The names of the inputs used for validation
	 * @param train_size The size of the training set
	 * @param validation_size The size of the validation set
	 * @param total_time The total time taken for training
	 * @param optimiser The optimiser used for training
	 */
	void log_epoch_metrics(size_t epoch, const Eigen::MatrixXf& train_inputs, const Eigen::MatrixXf& train_targets,
						   const Eigen::MatrixXf& validation_inputs, const Eigen::MatrixXf& validation_targets, Dataset& dataset,
						   std::ofstream& log_file, const std::string& file_name);

	/**
	 * @brief Log the final results of the training
	 * @param log_file The file to log the results to
	 * @param dataset The dataset used for training
	 * @param train_inputs The inputs used for training
	 * @param train_targets The targets used for training
	 * @param train_names The names of the inputs used for training
	 * @param validation_inputs The inputs used for validation
	 * @param validation_targets The targets used for validation
	 * @param validation_names The names of the inputs used for validation
	 * @param train_size The size of the training set
	 * @param validation_size The size of the validation set
	 * @param total_time The total time taken for training
	 * @param optimiser The optimiser used for training
	 */
	void log_final_results(std::ofstream& log_file, Dataset& dataset, const Eigen::MatrixXf& train_inputs,
						   const Eigen::MatrixXf& train_targets, const std::vector<std::string>& train_names,
						   const Eigen::MatrixXf& validation_inputs, const Eigen::MatrixXf& validation_targets,
						   const std::vector<std::string>& validation_names, size_t train_size, size_t validation_size,
						   std::chrono::duration<double> total_time, IOptimiser& optimiser);
};

#endif // NEURAL_NETWORK_HPP