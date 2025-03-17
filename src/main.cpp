/**
 * @file src/main.cpp
 * The main file of the program. Where the neural network is trained.
 */

#include "dataset/dataset.hpp"
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
	// Comparaison on different encoders
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
								  std::make_unique<CyclesBoundedNormaliserEncoder>());

		std::cout << "Dataset loaded" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train(dataset, 2000, 0.75, "training_results/bounded_normaliser_encoder", 2);
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
								  std::make_unique<CyclesSqrtEncoder>(0));

		std::cout << "Dataset loaded" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train(dataset, 2000, 0.75, "training_results/sqrt_encoder", 2);
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
								  std::make_unique<CyclesSqrtEncoder>(10));

		std::cout << "Dataset loaded" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train(dataset, 2000, 0.75, "training_results/sqrt_p10_encoder", 2);
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
								  std::make_unique<CyclesSqrtEncoder>(100));

		std::cout << "Dataset loaded" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train(dataset, 2000, 0.75, "training_results/sqrt_p100_encoder", 2);
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
								  std::make_unique<CyclesSqrtEncoder>(1000));

		std::cout << "Dataset loaded" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train(dataset, 2000, 0.75, "training_results/sqrt_p1000_encoder", 2);
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
								  std::make_unique<CyclesLogEncoder>(2, 0));

		std::cout << "Dataset loaded" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train(dataset, 2000, 0.75, "training_results/log2_encoder", 2);
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
								  std::make_unique<CyclesLogEncoder>(10, 0));

		std::cout << "Dataset loaded" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train(dataset, 2000, 0.75, "training_results/log10_encoder", 2);
	}
	{
		std::vector<size_t> topology = {
			MAX_TOKENS_NN / 8,
			(MAX_TOKENS_NN * 2) / 8,
			(MAX_TOKENS_NN / 2) / 8,
			(MAX_TOKENS_NN / 8) / 8,
			4,
		};
		NeuralNetwork nn(topology, 0.02, std::make_unique<Sigmoid>());
		std::cout << "Neural network created" << '\n';

		Dataset dataset = Dataset(new HexadecimalParser(),
								  "dataset/bench_bins_small",
								  topology,
								  std::make_unique<SizeEncoder>(topology[0]),
								  std::make_unique<CyclesSplitterEncoder>());

		std::cout << "Dataset loaded" << '\n';

		std::cout << "Training the neural network" << '\n';
		nn.train(dataset, 2000, 0.75, "training_results/splitter_encoder", 2);
	}

	exit(EXIT_SUCCESS);
}