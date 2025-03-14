/**
 * @file include/neural_network/layer.hpp
 * @brief The header file for a neural network layer
 */

#ifndef LAYER_HPP
#define LAYER_HPP

#include <eigen3/Eigen/Dense>

/**
 * @brief Class representing a neural network layer
 */
class Layer {
  public:
	/**
	 * @brief Construct a new Layer object
	 * @param input_size The size of the input the layer takes in
	 * @param output_size The size of the output the layer produces
	 */
	Layer(int input_size, int output_size);

	/**
	 * @brief Feed forward the input through the layer
	 * @param input The input to the layer
	 * @return The output of the layer
	 */
	Eigen::MatrixXf feed_forward(Eigen::MatrixXf* input);

	/**
	 * @brief Back propagate the error through the layer
	 * @param error The error to back propagate
	 * @param learning_rate The learning rate of the network
	 * @param next_values The values of the next layer
	 * @return The error to back propagate to the previous layer
	 */
	Eigen::MatrixXf back_propagate(const Eigen::MatrixXf& error, float learning_rate, Eigen::MatrixXf& next_values);

  private:
	Eigen::MatrixXf m_weights; ///< Matrix of the weights of the connections to that layer
	Eigen::MatrixXf m_biases;  ///< Matrix of the biases of the neurons in that layer
	Eigen::MatrixXf m_values;  ///< Matrix of the values of the neurons in that layer

	friend class NeuralNetwork; ///< Allow the neural network to access the private members
};
#endif // LAYER_HPP