/**
 * @file include/neural_network/neural_network.hpp
 * @brief The header file for the neural network
 */

#ifndef NEURAL_NETWORK_HPP
#define NEURAL_NETWORK_HPP
#include "dataset/dataset.hpp"
#include "neural_network/activation_functions.hpp"
#include "neural_network/layer.hpp"
#include "parsing/iencoder.hpp"
#include <eigen3/Eigen/Dense>
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Class representing a neural network
 */
class NeuralNetwork {
  private:
	std::vector<size_t> m_topology;					   ///< Number of neurons in each layer
	std::vector<Layer> m_layers;					   ///< Vector of layers in the network
	Eigen::MatrixXf m_last_values;					   ///< Values of the last layer
	float m_learning_rate;							   ///< Learning rate of the network
	std::unique_ptr<ActivationFunc> m_activation_func; ///< Activation function of the network

	/**
	 * @brief Feed forward the input through the network
	 * @param input The input to the network
	 */
	void feed_forward(const Eigen::VectorXf* input);

	/**
	 * @brief Back propagate the error through the network
	 * @param target_output The target output
	 */
	void back_propagate(const Eigen::VectorXf* target_output);

	/**
	 * @brief Calculate the squared error between the prediction and the target
	 * @param prediction The prediction output of the network
	 * @param target The target output
	 * @param encoder The encoder used to encode the data
	 * @return The squared error
	 */
	double squared_error(const Eigen::VectorXf* prediction, const Eigen::VectorXf* target, IEncoder* encoder);

	/**
	 * @brief Calculate the relative (normalised) squared error between the prediction and the target
	 * @param prediction The prediction output of the network
	 * @param target The target output
	 * @param encoder The encoder used to encode the data
	 * @return The relative squared error
	 */
	double relative_squared_error(const Eigen::VectorXf* prediction, const Eigen::VectorXf* target, IEncoder* encoder);

	/**
	 * @brief Calculate the absolute error between the prediction and the target
	 * @param prediction The prediction of the network
	 * @param target The target output
	 * @param encoder The encoder used to encode the data
	 * @return The absolute error
	 */
	double absolute_error(const Eigen::VectorXf* prediction, const Eigen::VectorXf* target, IEncoder* encoder);

	/**
	 * @brief Calculate the relative absolute error between the prediction and the target
	 * @param prediction The prediction of the network
	 * @param target The target output
	 * @param encoder The encoder used to encode the data
	 * @return The absolute error
	 */
	double relative_absolute_error(const Eigen::VectorXf* prediction, const Eigen::VectorXf* target, IEncoder* encoder);

  public:
	/**
	 * @brief Construct a new NeuralNetwork object
	 * @param topology The number of neurons in each layer
	 * @param learning_rate The learning rate of the network
	 */
	NeuralNetwork(std::vector<size_t>& topology, float learning_rate, std::unique_ptr<ActivationFunc> activation_func);

	/**
	 * @brief Train the neural network
	 * @param dataset The dataset to train on
	 * @param nb_epochs The number of epochs to train for
	 * @param training_proportion the proportion of inputs to be used as training data (from 0 for no training data, to 1 for all training
	 * data), the rest will be used as validation data
	 * @param logging_dir The directory to save the logs
	 * @param nb_trains The number of times to train the network
	 */
	void train(Dataset& dataset, int nb_epochs, float training_proportion, std::string&& logging_dir, int nb_trains);

	/**
	 * @brief Get the prediction of the network
	 * @param input The input to the network
	 * @return The prediction of the network
	 */
	Eigen::VectorXf get_prediction(const Eigen::VectorXf* input);
	std::vector<float> get_prediction(const std::vector<float>& input);

	/**
	 * @brief Convert input data and target data to correct format to compute loss and accuracy
	 * @param inputs The input data
	 * @param targets The target output data
	 * @return The loss of the network
	 */
	std::pair<std::vector<Eigen::VectorXf*>, std::vector<Eigen::VectorXf*>> convert_data_for_loss(std::vector<std::vector<float>>& inputs,
																								  std::vector<std::vector<float>>& targets);
	std::pair<std::vector<Eigen::VectorXf*>, std::vector<Eigen::VectorXf*>> convert_data_for_loss(std::vector<Eigen::VectorXf>& inputs,
																								  std::vector<Eigen::VectorXf>& targets);
	std::pair<std::vector<Eigen::VectorXf*>, std::vector<Eigen::VectorXf*>> convert_data_for_loss(Dataset& dataset);

	/**
	 * @brief Get the loss of the network, uses mean relative squared error
	 * @param inputs The input data
	 * @param targets The target output data
	 * @return The loss of the network
	 */
	double get_loss_mrse(std::vector<Eigen::VectorXf*>& inputs, std::vector<Eigen::VectorXf*>& targets, IEncoder* encoder);

	/**
	 * @brief Get the loss of the network, uses mean squared error
	 * @param inputs The input data
	 * @param targets The target output data
	 */
	double get_loss_mse(std::vector<Eigen::VectorXf*>& inputs, std::vector<Eigen::VectorXf*>& targets, IEncoder* encoder);

	/**
	 * @brief Get the accuracy of the network, uses mean absolute error
	 * @param inputs The input data
	 * @param targets The target output data
	 * @return The accuracy of the network
	 */
	double get_acc_mae(std::vector<Eigen::VectorXf*>& inputs, std::vector<Eigen::VectorXf*>& targets, IEncoder* encoder);

	/**
	 * @brief Get the accuracy of the network, uses mean relative absolute error
	 * @param inputs The input data
	 * @param targets The target output data
	 * @return The accuracy of the network
	 */
	double get_acc_mrae(std::vector<Eigen::VectorXf*>& inputs, std::vector<Eigen::VectorXf*>& targets, IEncoder* encoder);
};

#endif // NEURAL_NETWORK_HPP