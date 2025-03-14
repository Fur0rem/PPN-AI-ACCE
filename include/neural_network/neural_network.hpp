/**
 * @file include/neural_network/neural_network.hpp
 * @brief The header file for the neural network
 */

#ifndef NEURAL_NETWORK_HPP
#define NEURAL_NETWORK_HPP
#include "dataset/dataset.hpp"
#include "neural_network/layer.hpp"
#include <eigen3/Eigen/Dense>
#include <string>
#include <vector>

/**
 * @brief Class representing a neural network
 */
class NeuralNetwork {
  private:
	std::vector<size_t> m_topology; ///< Number of neurons in each layer
	std::vector<Layer> m_layers;	///< Vector of layers in the network
	Eigen::MatrixXf m_last_values;	///< Values of the last layer
	float m_learning_rate;			///< Learning rate of the network

	/**
	 * @brief Feed forward the input through the network
	 * @param input The input to the network
	 */
	void feed_forward(const Eigen::VectorXf* input);

	/**
	 * @brief Back propagate the error through the network
	 * @param target_output The target output of the network
	 */
	void back_propagate(const Eigen::VectorXf* target_output);

  public:
	/**
	 * @brief Construct a new NeuralNetwork object
	 * @param topology The number of neurons in each layer
	 * @param learning_rate The learning rate of the network
	 */
	NeuralNetwork(std::vector<size_t>& topology, float learning_rate);

	/**
	 * @brief Train the neural network
	 * @param inputs The input data
	 * @param targets The target output data
	 * @param nb_epochs The number of epochs to train for
	 * @param input_ratio the ratio of inputs to be used as training data, the rest will be used as validation data
	 * @param logging_filename The name of the file to log the loss to
	 */
	void train(Dataset& dataset, int nb_epochs, float input_ratio, std::string&& logging_filename);

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
	double get_loss_mrse(std::vector<Eigen::VectorXf*>& inputs, std::vector<Eigen::VectorXf*>& targets);

	/**
	 * @brief Get the loss of the network, uses mean squared error
	 * @param inputs The input data
	 * @param targets The target output data
	 */
	double get_loss_mse(std::vector<Eigen::VectorXf*>& inputs, std::vector<Eigen::VectorXf*>& targets);

	/**
	 * @brief Get the accuracy of the network, uses mean absolute error
	 * @param inputs The input data
	 * @param targets The target output data
	 * @return The accuracy of the network
	 */
	double get_acc_mae(std::vector<Eigen::VectorXf*>& inputs, std::vector<Eigen::VectorXf*>& targets);

	/**
	 * @brief Get the accuracy of the network, uses mean relative absolute error
	 * @param inputs The input data
	 * @param targets The target output data
	 * @return The accuracy of the network
	 */
	double get_acc_mrae(std::vector<Eigen::VectorXf*>& inputs, std::vector<Eigen::VectorXf*>& targets);
};

#endif // NEURAL_NETWORK_HPP