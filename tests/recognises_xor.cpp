/**
 * @file tests/recognises_xor.cpp
 * @brief Used to test the neural network implementation on xor example.
 */

#include "neural_network/neural_network.hpp"
#include <gtest/gtest.h>
#include <iostream>

/**
 * @brief Test to check if the neural network can recognise the xor function
 */
TEST(NeuralNetwork, Xor) {
	// Creating neural network
	// 2 input neurons, 4 hidden neurons and 1 output neuron
	std::vector<size_t> topology = {2, 8, 1};
	NeuralNetwork nn(topology, 0.2);

	// Sample dataset
	std::vector<std::vector<float>> target_inputs = {{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}};
	std::vector<std::vector<float>> target_outputs = {{0.0f}, {1.0f}, {1.0f}, {0.0f}};

	Dataset dataset = Dataset(target_inputs, target_outputs, topology);

	int nb_epochs = 10000;
	std::cout << "Training the neural network" << '\n';

	nn.train(dataset, nb_epochs, 1.0, "xor.log");

	std::cout << "Training complete" << '\n';

	// Testing the neural network
	for (std::vector<float> input : target_inputs) {
		std::vector<float> output = nn.get_prediction(input);
		std::cout << "Input: " << input[0] << " " << input[1] << " Output: " << output[0] << '\n';
		EXPECT_NEAR(output[0], input[0] != input[1], 0.05);
	}
}