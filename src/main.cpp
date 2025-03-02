/**
 * @file main.cpp
 * The main file of the program.
 */

#include "dataset/dataset.hpp"
#include "neural_network/neural_network.hpp"
#include "parsing/binary_with_split_parser.hpp"
#include "parsing/rawbinary_parser.hpp"
#include "parsing/xor_parser.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>

/**
 * @brief The main function.
 * @return The exit status of the program.
 */
int main() {
	// Creating neural network
	std::vector<int> topology = {
		MAX_TOKENS_NN,
		3000,
		2000,
		500,
		1,
	};
	NeuralNetwork nn(topology, 0.2);
	std::cout << "Neural network created" << '\n';

	// Dataset dataset = Dataset(new XORParser(), "dataset/xor");
	Dataset dataset = Dataset(new RawBinaryParser(), "dataset/asm");
	std::cout << "Dataset loaded" << '\n';

	int nb_epochs = 1000;
	std::cout << "Training the neural network" << '\n';

	auto data = dataset.get_data(1.0);
	std::vector<std::vector<float>> target_inputs;
	std::vector<std::vector<float>> target_outputs;
	for (auto [input, output] : data) {
		// turn Eigen::VectorXf into std::vector<float>
		std::vector<float> input_vec;
		std::vector<float> output_vec;
		for (int i = 0; i < input.size(); i++) {
			input_vec.push_back(input(i));
		}
		for (int i = 0; i < output.size(); i++) {
			output_vec.push_back(output(i));
		}
		target_inputs.push_back(input_vec);
		target_outputs.push_back(output_vec);
	}

	nn.train(target_inputs, target_outputs, nb_epochs, 1.0, "asm_training.log");

	std::cout << "Training complete" << '\n';

	// Testing the neural network
	for (std::vector<float> input : target_inputs) {
		std::vector<float> output = nn.get_prediction(input);
	}

	std::cout << "Total loss : " << nn.get_total_loss(target_inputs, target_outputs) << '\n';
}