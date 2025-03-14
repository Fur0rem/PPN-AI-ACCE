/**
 * @file include/neural_network/activation_functions.hpp
 * @brief Header for the activation functions used in the neural network
 */

#ifndef ACITVATION_FUNCTIONS_HPP
#define ACITVATION_FUNCTIONS_HPP

/**
 * @brief The sigmoid activation function
 * @param x The input to the function
 * @return The output of the function
 */
float sigmoid(float x);

/**
 * @brief The derivative of the sigmoid activation function
 * @param x The input to the function
 * @return The output of the function
 */
float deriv_sigmoid(float x);

/**
 * @brief The ReLU activation function
 * @param x The input to the function
 * @return The output of the function
 */
float relu(float x);

/**
 * @brief The derivative of the ReLU activation function
 * @param x The input to the function
 * @return The output of the function
 */
float deriv_relu(float x);

/**
 * @brief The leaky ReLU activation function
 * @param x The input to the function
 * @return The output of the function
 */
float leaky_relu(float x);

/**
 * @brief The derivative of the leaky ReLU activation function
 * @param x The input to the function
 * @return The output of the function
 */
float deriv_leaky_relu(float x);

// tanh is already defined in the std library

/**
 * @brief The derivative of the tanh activation function
 * @param x The input to the function
 * @return The output of the function
 */
float deriv_tanh(float x);

#endif // ACITVATION_FUNCTIONS_HPP