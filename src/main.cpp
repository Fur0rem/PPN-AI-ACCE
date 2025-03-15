/**
 * @file main.cpp
 * The main file of the program.
 */

#include "dataset/dataset.hpp"
#include "neural_network/neural_network.hpp"
#include "parsing/binary_parser.hpp"
#include "parsing/binary_with_split_parser.hpp"
#include "parsing/cycles_normaliser_encoder.hpp"
#include "parsing/hexadecimal_parser.hpp"
#include "parsing/hexadecimal_with_split_parser.hpp"
#include "parsing/non_encoder.hpp"
#include "parsing/size_encoder.hpp"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

/**
 * @brief The main function.
 * @return The exit status of the program.
 */
int main() {
	// Compare the 4 formats using a smaller part of the dataset and less epochs
	{
		std::vector<size_t> topology = {
			MAX_TOKENS_NN / 8,
			(MAX_TOKENS_NN * 2) / 8,
			(MAX_TOKENS_NN / 2) / 8,
			(MAX_TOKENS_NN / 8) / 8,
			1,
		};
		NeuralNetwork nn(topology, 0.02, std::make_unique<Sigmoid>());
		std::cout << "Neural network created" << '\n';

		Dataset dataset = Dataset(new HexadecimalParser(),
								  "dataset/bench_bins_small",
								  topology,
								  std::make_unique<SizeEncoder>(topology[0]),
								  std::make_unique<CyclesNormaliserEncoder>());
		std::cout << "Dataset loaded" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train(dataset, 300, 0.75, "training_results/hexadecimal_format", 2);
		std::cout << "Training complete" << '\n';
	}
	{
		std::vector<size_t> topology = {
			MAX_TOKENS_NN / 7,
			(MAX_TOKENS_NN * 2) / 7,
			(MAX_TOKENS_NN / 2) / 7,
			(MAX_TOKENS_NN / 8) / 7,
			1,
		};
		NeuralNetwork nn(topology, 0.02, std::make_unique<Sigmoid>());
		std::cout << "Neural network created" << '\n';

		Dataset dataset = Dataset(new HexadecimalWithSplitParser(),
								  "dataset/bench_bins_small",
								  topology,
								  std::make_unique<SizeEncoder>(topology[0]),
								  std::make_unique<CyclesNormaliserEncoder>());
		std::cout << "Dataset loaded" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train(dataset, 300, 0.75, "training_results/hexadecimal_with_split_format", 2);
		std::cout << "Training complete" << '\n';
	}
	{
		std::vector<size_t> topology = {
			MAX_TOKENS_NN,
			MAX_TOKENS_NN * 2,
			MAX_TOKENS_NN / 2,
			MAX_TOKENS_NN / 8,
			1,
		};
		NeuralNetwork nn(topology, 0.02, std::make_unique<Sigmoid>());
		std::cout << "Neural network created" << '\n';

		Dataset dataset = Dataset(new BinaryParser(),
								  "dataset/bench_bins_small",
								  topology,
								  std::make_unique<SizeEncoder>(topology[0]),
								  std::make_unique<CyclesNormaliserEncoder>());
		std::cout << "Dataset loaded" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train(dataset, 300, 0.75, "training_results/binary_format", 2);
		std::cout << "Training complete" << '\n';
	}
	{
		std::vector<size_t> topology = {
			MAX_TOKENS_NN + MAX_TOKENS_NN / 64,
			MAX_TOKENS_NN * 2 + MAX_TOKENS_NN / 64,
			MAX_TOKENS_NN / 2 + MAX_TOKENS_NN / 64,
			MAX_TOKENS_NN / 8 + MAX_TOKENS_NN / 64,
			1,
		};
		NeuralNetwork nn(topology, 0.02, std::make_unique<Sigmoid>());
		std::cout << "Neural network created" << '\n';

		Dataset dataset = Dataset(new BinaryWithSplitParser(),
								  "dataset/bench_bins_small",
								  topology,
								  std::make_unique<SizeEncoder>(topology[0]),
								  std::make_unique<CyclesNormaliserEncoder>());
		std::cout << "Dataset loaded" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train(dataset, 300, 0.75, "training_results/binary_with_split_format", 2);
		std::cout << "Training complete" << '\n';
	}

	// Comparaison on the full set of HexadecimalWithSplit parser and Hexadecimal parser
	{
		std::vector<size_t> topology = {
			MAX_TOKENS_NN / 7,
			(MAX_TOKENS_NN * 2) / 7,
			(MAX_TOKENS_NN / 2) / 7,
			(MAX_TOKENS_NN / 8) / 7,
			1,
		};
		NeuralNetwork nn(topology, 0.02, std::make_unique<Sigmoid>());
		std::cout << "Neural network created" << '\n';

		Dataset dataset = Dataset(new HexadecimalWithSplitParser(),
								  "dataset/bench_bins",
								  topology,
								  std::make_unique<SizeEncoder>(topology[0]),
								  std::make_unique<CyclesNormaliserEncoder>());
		std::cout << "Dataset loaded" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train(dataset, 1000, 0.75, "training_results/hexadecimal_with_split_format_full_train", 3);
		std::cout << "Training complete" << '\n';
	}
	{
		std::vector<size_t> topology = {
			MAX_TOKENS_NN / 8,
			(MAX_TOKENS_NN * 2) / 8,
			(MAX_TOKENS_NN / 2) / 8,
			(MAX_TOKENS_NN / 8) / 8,
			1,
		};
		NeuralNetwork nn(topology, 0.02, std::make_unique<Sigmoid>());
		std::cout << "Neural network created" << '\n';

		Dataset dataset = Dataset(new HexadecimalParser(),
								  "dataset/bench_bins_small",
								  topology,
								  std::make_unique<SizeEncoder>(topology[0]),
								  std::make_unique<CyclesNormaliserEncoder>());
		std::cout << "Dataset loaded" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train(dataset, 1000, 0.75, "training_results/hexadecimal_format_full_training", 3);
		std::cout << "Training complete" << '\n';
	}
}