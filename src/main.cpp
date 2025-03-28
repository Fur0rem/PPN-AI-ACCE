/**
 * @file src/main.cpp
 * The main file of the program. Where the neural network is trained.
 */

#include "dataset/dataset.hpp"
#include "neural_network/activation_functions.hpp"
#include "neural_network/neural_network.hpp"
#include "parsing/binary_parser.hpp"
#include "parsing/binary_with_split_parser.hpp"
#include "parsing/cycles_encoders.hpp"
#include "parsing/hexadecimal_parser.hpp"
#include "parsing/hexadecimal_with_split_parser.hpp"
#include "parsing/non_encoder.hpp"
#include "parsing/size_encoder.hpp"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

/**
 * @brief The main function. Used to train the neural network.
 * @return The exit status of the program.
 */
int main() {
	// Comparaison on different learning rates for batch training
	std::vector<size_t> topology = {
		MAX_TOKENS_NN / 8,
		MAX_TOKENS_NN / 8,
		MAX_TOKENS_NN / 32,
		1,
	};

	Dataset dataset = Dataset(new HexadecimalParser(),
							  "dataset/bench_bins",
							  topology,
							  std::make_unique<SizeEncoder>(topology[0]),
							  std::make_unique<CyclesLogEncoder>(2, 0));

	std::cout << "Dataset loaded" << '\n';
	// 0.0005
	{
		NeuralNetwork nn(topology, 0.0005, std::make_unique<Sigmoid>());
		std::cout << "Neural network created" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train_batch(dataset, 8000, 0.75, 0.25, "training_results/batch_training_0_0005", 1);
	}

	// 0.001
	{
		NeuralNetwork nn(topology, 0.001, std::make_unique<Sigmoid>());
		std::cout << "Neural network created" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train_batch(dataset, 8000, 0.75, 0.25, "training_results/batch_training_0_001", 1);
	}

	// 0.01
	{
		NeuralNetwork nn(topology, 0.01, std::make_unique<Sigmoid>());
		std::cout << "Neural network created" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train_batch(dataset, 8000, 0.75, 0.25, "training_results/batch_training_0_01", 1);
	}

	// 0.02
	{
		NeuralNetwork nn(topology, 0.02, std::make_unique<Sigmoid>());
		std::cout << "Neural network created" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train_batch(dataset, 8000, 0.75, 0.25, "training_results/batch_training_0_02", 1);
	}

	// 0.03
	{
		NeuralNetwork nn(topology, 0.03, std::make_unique<Sigmoid>());
		std::cout << "Neural network created" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train_batch(dataset, 8000, 0.75, 0.25, "training_results/batch_training_0_03", 1);
	}

	// 0.05
	{
		NeuralNetwork nn(topology, 0.05, std::make_unique<Sigmoid>());
		std::cout << "Neural network created" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train_batch(dataset, 8000, 0.75, 0.25, "training_results/batch_training_0_05", 1);
	}

	// 0.1
	{
		NeuralNetwork nn(topology, 0.1, std::make_unique<Sigmoid>());
		std::cout << "Neural network created" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train_batch(dataset, 8000, 0.75, 0.25, "training_results/batch_training_0_1", 1);
	}

	// 0.15
	{
		NeuralNetwork nn(topology, 0.15, std::make_unique<Sigmoid>());
		std::cout << "Neural network created" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train_batch(dataset, 8000, 0.75, 0.25, "training_results/batch_training_0_15", 8);
	}

	// 0.25
	{
		NeuralNetwork nn(topology, 0.25, std::make_unique<Sigmoid>());
		std::cout << "Neural network created" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train_batch(dataset, 8000, 0.75, 0.25, "training_results/batch_training_0_2", 1);
	}

	exit(EXIT_SUCCESS);
}