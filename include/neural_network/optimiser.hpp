/**
 * @file include/neural_network/optimiser.hpp
 * @brief Interface for optimisers during training of neural networks. Also contains a few optimiser implementations.
 */

#ifndef OPTIMISER_HPP
#define OPTIMISER_HPP

#include <eigen3/Eigen/Dense>

// Forward declaration of classes to avoid circular dependency
class Gradients;
class NeuralNetwork;

/**
 * @brief Interface for an optimiser
 */
class IOptimiser {
  public:
	/**
	 * @brief Update the weights and biases of the neural network using the gradients
	 * @param grads Gradients for weights and biases
	 * @param nn Neural network to update
	 */
	virtual void update_weights(const Gradients& grads, NeuralNetwork& nn) = 0;

	/**
	 * @brief Virtual destructor
	 */
	virtual ~IOptimiser() {}

	/**
	 * @brief Get the learning rate of the optimiser
	 * @return Learning rate
	 */
	virtual float get_learning_rate() const = 0;
};

/**
 * @brief Stochastic Gradient Descent (SGD) optimiser
 * @details This class implements the SGD optimisation algorithm for updating the weights and biases of a neural network.
 * It uses an average of the gradients of the batch to update the weights and biases.
 */
class SGD : public IOptimiser {
  private:
	float m_learning_rate;			   ///< Learning rate for the optimiser
	std::vector<Eigen::MatrixXf> m_vW; ///< Velocity for weights
	std::vector<Eigen::MatrixXf> m_vB; ///< Velocity for biases

  public:
	/**
	 * @brief Constructor for SGD optimiser
	 * @param learning_rate Learning rate for the optimiser
	 */
	SGD(float learning_rate);

	/**
	 * @brief Update the weights and biases of the neural network using the gradients
	 * @param grads Gradients for weights and biases
	 * @param nn Neural network to update
	 */
	void update_weights(const Gradients& grads, NeuralNetwork& nn) override;

	/**
	 * @brief Virtual destructor
	 */
	~SGD();

	/**
	 * @brief Get the learning rate of the optimiser
	 * @return Learning rate
	 */
	float get_learning_rate() const override;
};

/**
 * @brief Adam optimiser
 * @details This class implements the Adam optimisation algorithm for updating the weights and biases of a neural network.
 * It uses an adaptive learning rate based on the first and second moments of the gradients.
 */
class Adam : public IOptimiser {
  private:
	float m_learning_rate;			   ///< Learning rate for the optimiser
	float m_beta1;					   ///< Exponential decay rate for the first moment
	float m_beta2;					   ///< Exponential decay rate for the second moment
	float m_epsilon;				   ///< Small constant to prevent division by zero
	std::vector<Eigen::MatrixXf> m_mW; ///< First moment for weights
	std::vector<Eigen::MatrixXf> m_vW; ///< Second moment for weights
	std::vector<Eigen::MatrixXf> m_mB; ///< First moment for biases
	std::vector<Eigen::MatrixXf> m_vB; ///< Second moment for biases

  public:
	/**
	 * @brief Constructor for Adam optimiser
	 * @param nn Neural network getting trained
	 * @param beta1 Exponential decay rate for the first moment
	 * @param beta2 Exponential decay rate for the second moment
	 * @param epsilon Small constant to prevent division by zero
	 * @param learning_rate Learning rate for the optimiser
	 */
	Adam(NeuralNetwork& nn, float beta1, float beta2, float epsilon, float learning_rate);

	/**
	 * @brief Update the weights and biases of the neural network using the gradients
	 * @param grads Gradients for weights and biases
	 * @param nn Neural network to update
	 */
	void update_weights(const Gradients& grads, NeuralNetwork& nn) override;

	/**
	 * @brief Virtual destructor
	 */
	~Adam();

	/**
	 * @brief Get the learning rate of the optimiser
	 * @return Learning rate
	 */
	float get_learning_rate() const override;
};

/**
 * @brief Momentum optimiser
 * @details This class implements the Momentum optimisation algorithm for updating the weights and biases of a neural network.
 * It uses a moving average of the gradients to update the weights and biases.
 */
class Momentum : public IOptimiser {
  private:
	float m_beta;					   ///< Exponential decay rate for the moving average
	std::vector<Eigen::MatrixXf> m_vW; ///< Moving average for weights
	std::vector<Eigen::MatrixXf> m_vB; ///< Moving average for biases
	float m_learning_rate;			   ///< Learning rate for the optimiser

  public:
	/**
	 * @brief Constructor for Momentum optimiser
	 * @param nn Neural network getting trained
	 * @param beta Exponential decay rate for the moving average
	 * @param learning_rate Learning rate for the optimiser
	 */
	Momentum(NeuralNetwork& nn, float beta, float learning_rate);

	/**
	 * @brief Update the weights and biases of the neural network using the gradients
	 * @param grads Gradients for weights and biases
	 * @param nn Neural network to update
	 */
	void update_weights(const Gradients& grads, NeuralNetwork& nn) override;

	/**
	 * @brief Virtual destructor
	 */
	~Momentum();

	/**
	 * @brief Get the learning rate of the optimiser
	 * @return Learning rate
	 */
	float get_learning_rate() const override;
};

/**
 * @brief RMSProp optimiser
 * @details This class implements the RMSProp optimisation algorithm for updating the weights and biases of a neural network.
 * It uses an adaptive learning rate based on the moving average of the squared gradients.
 */
class RMSProp : public IOptimiser {
  private:
	float m_beta;					   ///< Exponential decay rate for the moving average
	float m_epsilon;				   ///< Small constant to prevent division by zero
	std::vector<Eigen::MatrixXf> m_vW; ///< Moving average for weights
	std::vector<Eigen::MatrixXf> m_vB; ///< Moving average for biases
	float m_learning_rate;			   ///< Learning rate for the optimiser

  public:
	/**
	 * @brief Constructor for RMSProp optimiser
	 * @param nn Neural network getting trained
	 * @param beta Exponential decay rate for the moving average
	 * @param epsilon Small constant to prevent division by zero
	 * @param learning_rate Learning rate for the optimiser
	 */
	RMSProp(NeuralNetwork& nn, float beta, float epsilon, float learning_rate);

	/**
	 * @brief Update the weights and biases of the neural network using the gradients
	 * @param grads Gradients for weights and biases
	 * @param nn Neural network to update
	 */
	void update_weights(const Gradients& grads, NeuralNetwork& nn) override;

	/**
	 * @brief Virtual destructor
	 */
	~RMSProp();

	/**
	 * @brief Get the learning rate of the optimiser
	 * @return Learning rate
	 */
	float get_learning_rate() const override;
};

/**
 * @brief AMSGrad optimiser
 * @details This class implements the AMSGrad optimisation algorithm for updating the weights and biases of a neural network.
 * It uses an adaptive learning rate based on the maximum of the moving average of the squared gradients.
 */
class AMSGrad : public IOptimiser {
  private:
	std::vector<Eigen::MatrixXf> m_mW;	   ///< First moment for weights
	std::vector<Eigen::MatrixXf> m_mB;	   ///< First moment for biases
	std::vector<Eigen::MatrixXf> m_vW;	   ///< Second moment for weights
	std::vector<Eigen::MatrixXf> m_vB;	   ///< Second moment for biases
	std::vector<Eigen::MatrixXf> m_vW_hat; ///< Maximum of the moving average for weights
	std::vector<Eigen::MatrixXf> m_vB_hat; ///< Maximum of the moving average for biases
	float m_beta1;						   ///< Exponential decay rate for the first moment
	float m_beta2;						   ///< Exponential decay rate for the second moment
	float m_epsilon;					   ///< Small constant to prevent division by zero
	float m_learning_rate;				   ///< Learning rate for the optimiser

  public:
	/**
	 * @brief Constructor for AMSGrad optimiser
	 * @param nn Neural network getting trained
	 * @param beta1 Exponential decay rate for the first moment
	 * @param beta2 Exponential decay rate for the second moment
	 * @param epsilon Small constant to prevent division by zero
	 * @param learning_rate Learning rate for the optimiser
	 */
	AMSGrad(NeuralNetwork& nn, float beta1, float beta2, float epsilon, float learning_rate);

	/**
	 * @brief Update the weights and biases of the neural network using the gradients
	 * @param grads Gradients for weights and biases
	 * @param nn Neural network to update
	 */
	void update_weights(const Gradients& grads, NeuralNetwork& nn) override;

	/**
	 * @brief Virtual destructor
	 */
	~AMSGrad();

	/**
	 * @brief Get the learning rate of the optimiser
	 * @return Learning rate
	 */
	float get_learning_rate() const override;
};

#endif // OPTIMISER_HPP