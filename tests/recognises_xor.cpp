/**
 * @file tests/recongises_xor.cpp
 * Used to test the neural network implementation on xor example.
 */

#include "neural_network/neural_network.hpp"
#include <gtest/gtest.h>
#include <iostream>

TEST(NeuralNetwork, Xor) {
	// Creating neural network
	// 2 input neurons, 3 hidden neurons and 1 output neuron
	std::vector<int> topology = {2, 3, 1};
	NeuralNetwork nn(topology, 0.1);

	// Sample dataset
	std::vector<std::vector<float>> target_inputs = {{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}};
	std::vector<std::vector<float>> target_outputs = {{0.0f}, {1.0f}, {1.0f}, {0.0f}};

	int nb_epochs = 100000;
	std::cout << "Training the neural network" << '\n';

	nn.train(target_inputs, target_outputs, nb_epochs, "xor.log");

	std::cout << "Training complete" << '\n';

	// Testing the neural network
	for (std::vector<float> input : target_inputs) {
		std::vector<float> output = nn.get_prediction(input);
		std::cout << "Input: " << input[0] << " " << input[1] << " Output: " << output[0] << '\n';
	}

	EXPECT_NEAR(nn.get_total_loss(target_inputs, target_outputs), 0.0, 1e-3);
}