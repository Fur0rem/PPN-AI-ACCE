/**
 * @file include/neural_network/activation_functions.hpp
 * @brief Header for the activation functions used in the neural network
 */

#ifndef ACITVATION_FUNCTIONS_HPP
#define ACITVATION_FUNCTIONS_HPP

/**
 * @brief The activation function class
 */
class ActivationFunc {
  public:
	/**
	 * @brief The activation function
	 * @param x The input to the function
	 * @return The output of the function
	 */
	virtual float func(float x) = 0;

	/**
	 * @brief The derivative of the activation function
	 * @param x The input to the function
	 * @return The output of the function
	 */
	virtual float deriv(float x) = 0;
};

/**
 * @brief The sigmoid activation function
 */
class Sigmoid : public ActivationFunc {
  public:
	float func(float x) override;
	float deriv(float x) override;
};

/**
 * @brief The ReLU activation function
 */
class ReLU : public ActivationFunc {
  public:
	float func(float x) override;
	float deriv(float x) override;
};

/**
 * @brief The Leaky ReLU activation function
 */
class LeakyReLU : public ActivationFunc {
  public:
	float func(float x) override;
	float deriv(float x) override;
};

/**
 * @brief The tanh activation function
 */
class Tanh : public ActivationFunc {
  public:
	float func(float x) override;
	float deriv(float x) override;
};

/**
 * @brief The eLU activation function
 */
class ELU : public ActivationFunc {
  public:
	float func(float x) override;
	float deriv(float x) override;
};

/**
 * @brief The GELU activation function
 */
class GELU : public ActivationFunc {
  public:
	float func(float x) override;
	float deriv(float x) override;
};

#endif // ACITVATION_FUNCTIONS_HPP