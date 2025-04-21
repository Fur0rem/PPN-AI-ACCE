/**
 * @file tests/recognises_xor.cpp
 * @brief Used to test the neural network implementation on xor example.
 */

#include "dataset/dataset.hpp"
#include "neural_network/neural_network.hpp"
#include "neural_network/optimiser.hpp"
#include "parsing/non_encoder.hpp"
#include <gtest/gtest.h>
#include <memory>

/**
 * @brief Test to check if the neural network can recognise the xor function using regular training
 */
TEST(NeuralNetwork, XorRegularTraining) {
	// Creating neural network
	// 2 input neurons, 3 hidden neurons and 1 output neuron
	std::vector<size_t> topology = {2, 4, 1};
	std::unique_ptr<TrainingNoise> training_noise = std::make_unique<TrainingNoise>(0.0F, 0.01F, 0.01F, 0.0F);
	NeuralNetwork nn(topology, std::make_unique<Sigmoid>(), std::move(training_noise));

	// Sample dataset
	std::vector<std::vector<float>> target_inputs = {{0.0F, 0.0F}, {0.0F, 1.0F}, {1.0F, 0.0F}, {1.0F, 1.0F}};
	std::vector<std::vector<float>> target_outputs = {{0.0F}, {1.0F}, {1.0F}, {0.0F}};

	Dataset dataset = Dataset(target_inputs, target_outputs, topology, std::make_unique<NonEncoder>(), std::make_unique<NonEncoder>());

	// Training the neural network
	nn.train(dataset, 20000, 1.0, 0.1, std::string("training_results/xor_regular"), 1);

	// Testing the neural network
	for (std::vector<float> input : target_inputs) {
		std::vector<float> output = nn.predict(input);
		EXPECT_NEAR(output[0], input[0] != input[1], 0.1);
	}
}

/**
 * @brief Test to check if the neural network can recognise the xor function using batch training
 */
TEST(NeuralNetwork, XorBatchTraining) {
	// Creating neural network
	// 2 input neurons, 3 hidden neurons and 1 output neuron
	std::vector<size_t> topology = {2, 3, 1};
	std::unique_ptr<TrainingNoise> training_noise = std::make_unique<TrainingNoise>(0.0F, 0.01F, 0.01F, 0.0F);
	NeuralNetwork nn(topology, std::make_unique<Sigmoid>(), std::move(training_noise));

	// Sample dataset
	std::vector<std::vector<float>> target_inputs = {{0.0F, 0.0F}, {0.0F, 1.0F}, {1.0F, 0.0F}, {1.0F, 1.0F}};
	std::vector<std::vector<float>> target_outputs = {{0.0F}, {1.0F}, {1.0F}, {0.0F}};

	Dataset dataset = Dataset(target_inputs, target_outputs, topology, std::make_unique<NonEncoder>(), std::make_unique<NonEncoder>());

	IOptimiser* optimiser = new Adam(nn, 0.3, 0.7, 1e-8, 0.05);
	nn.train_batch(dataset, 20000, 1.0, 4, *optimiser, "training_results/xor_batch", 1);
	delete optimiser;

	// Testing the neural network
	for (std::vector<float> input : target_inputs) {
		std::vector<float> output = nn.predict(input);
		EXPECT_NEAR(output[0], input[0] != input[1], 0.1);
	}
}
