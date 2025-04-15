/**
 * @file include/neural_network/gradients.hpp
 * @brief Header for the gradients used for training the neural network
 */

#ifndef GRADIENTS_HPP
#define GRADIENTS_HPP

#include <eigen3/Eigen/Dense>
#include <vector>

/**
 * @brief Class to hold gradients for weights and biases of a neural network
 */
class Gradients {
  private:
	std::vector<Eigen::MatrixXf> m_dW; ///< Gradients for weights of each layer
	std::vector<Eigen::MatrixXf> m_dB; ///< Gradients for biases of each layer

  public:
	/**
	 * @brief Constructor for Gradients class
	 * @param nb_layers Number of layers in the neural network
	 */
	Gradients(size_t nb_layers);

	/**
	 * @brief Get the gradients for weights
	 * @return Reference to the vector of gradients for weights
	 */
	std::vector<Eigen::MatrixXf>& d_w();

	/**
	 * @brief Get the gradients for biases
	 * @return Reference to the vector of gradients for biases
	 */
	std::vector<Eigen::MatrixXf>& d_b();

	/**
	 * @brief Get the gradients for weights (const version)
	 * @return Const reference to the vector of gradients for weights
	 */
	const std::vector<Eigen::MatrixXf>& d_w() const;

	/**
	 * @brief Get the gradients for biases (const version)
	 * @return Const reference to the vector of gradients for biases
	 */
	const std::vector<Eigen::MatrixXf>& d_b() const;
};

/**
 * @brief Mean Squared Error (MSE) loss function
 * @param prediction Predicted output
 * @param target Target output
 * @return MSE loss value
 */
float mse_loss(const Eigen::MatrixXf& prediction, const Eigen::MatrixXf& target);

/**
 * @brief Derivative of Mean Squared Error (MSE) loss function
 * @param prediction Predicted output
 * @param target Target output
 * @return Derivative of MSE loss value
 */
Eigen::MatrixXf mse_loss_derivative(const Eigen::MatrixXf& prediction, const Eigen::MatrixXf& target);

/**
 * @brief Cross-Entropy loss function
 * @param prediction Predicted output
 * @param target Target output
 * @return Cross-Entropy loss value
 */
float cross_entropy_loss(const Eigen::MatrixXf& prediction, const Eigen::MatrixXf& target);

/**
 * @brief Derivative of Cross-Entropy loss function
 * @param prediction Predicted output
 * @param target Target output
 * @return Derivative of Cross-Entropy loss value
 */
Eigen::MatrixXf cross_entropy_loss_derivative(const Eigen::MatrixXf& prediction, const Eigen::MatrixXf& target);

/**
 * @brief Mean Absolute Error (MAE) loss function
 * @param prediction Predicted output
 * @param target Target output
 * @return MAE loss value
 */
float mae_loss(const Eigen::MatrixXf& prediction, const Eigen::MatrixXf& target);

/**
 * @brief Derivative of Mean Absolute Error (MAE) loss function
 * @param prediction Predicted output
 * @param target Target output
 * @return Derivative of MAE loss value
 */
Eigen::MatrixXf mae_loss_derivative(const Eigen::MatrixXf& prediction, const Eigen::MatrixXf& target);

#endif // GRADIENTS_HPP