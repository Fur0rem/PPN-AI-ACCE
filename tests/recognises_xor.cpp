/**
 * @file tests/recognises_xor.cpp
 * @brief Used to test the neural network implementation on xor example.
 */

#include "dataset/dataset.hpp"
#include "neural_network/neural_network.hpp"
#include "parsing/non_encoder.hpp"
#include <gtest/gtest.h>
#include <memory>

/**
 * @brief Test to check if the neural network can recognise the xor function
 */
TEST(NeuralNetwork, Xor) {
	// Creating neural network
	// 2 input neurons, 3 hidden neurons and 1 output neuron
	std::vector<size_t> topology = {2, 3, 1};
	NeuralNetwork nn(topology, 0.2, std::make_unique<Sigmoid>());

	// Sample dataset
	std::vector<std::vector<float>> target_inputs = {{0.0F, 0.0F}, {0.0F, 1.0F}, {1.0F, 0.0F}, {1.0F, 1.0F}};
	std::vector<std::vector<float>> target_outputs = {{0.0F}, {1.0F}, {1.0F}, {0.0F}};

	Dataset dataset = Dataset(target_inputs, target_outputs, topology, std::make_unique<NonEncoder>(), std::make_unique<NonEncoder>());

	nn.train(dataset, 10000, 1.0, "training_results/xor", 3);

	// Testing the neural network
	for (std::vector<float> input : target_inputs) {
		std::vector<float> output = nn.get_prediction(input);
		EXPECT_NEAR(output[0], input[0] != input[1], 0.05);
	}
}