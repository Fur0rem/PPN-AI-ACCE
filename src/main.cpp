/**
 * @file main.cpp
 * The main file of the program.
 */

#include "dataset/dataset.hpp"
#include "neural_network/neural_network.hpp"
#include "parsing/asm_parser.hpp"
#include "parsing/binary_parser.hpp"
#include "parsing/binary_with_split_parser.hpp"
#include "parsing/hexadecimal_parser.hpp"
#include "parsing/hexadecimal_with_split_parser.hpp"
#include "parsing/xor_parser.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>

/**
 * @brief The main function.
 * @return The exit status of the program.
 */
int main() {
	{
		// Creating neural network
		std::vector<size_t> topology = {
			MAX_TOKENS_NN / 7,
			16000 / 7,
			5000 / 7,
			1,
		};
		NeuralNetwork nn(topology, 0.02);
		std::cout << "Neural network created" << '\n';

		Dataset dataset = Dataset(new HexadecimalWithSplitParser(), "dataset/bench_bins_small", topology);
		std::cout << "Dataset loaded" << '\n';

		int nb_epochs = 1000;
		std::cout << "Training the neural network" << '\n';

		nn.train(dataset, nb_epochs, 0.75, "training_results/hexa_split_ training.log");

		std::cout << "Training complete" << '\n';

		// Testing the neural network
		for (auto [input, target] : dataset.get_data(1.0)) {
			auto output = nn.get_prediction(&input);
			std::cout << "Prediction : " << output[0] << ", Target : " << target[0] << '\n';
		}

		auto [in, out] = nn.convert_data_for_loss(dataset);
		std::cout << "Total loss : " << nn.get_loss_mrse(in, out) << '\n';
	}
	{
		// Creating neural network
		std::vector<size_t> topology = {
			MAX_TOKENS_NN / 8,
			15000 / 8,
			5000 / 8,
			1,
		};
		NeuralNetwork nn(topology, 0.03);
		std::cout << "Neural network created" << '\n';

		Dataset dataset = Dataset(new HexadecimalParser(), "dataset/bench_bins_small", topology);
		std::cout << "Dataset loaded" << '\n';

		int nb_epochs = 1000;
		std::cout << "Training the neural network" << '\n';

		nn.train(dataset, nb_epochs, 0.75, "training_results/hexa_training.log");

		std::cout << "Training complete" << '\n';

		// Testing the neural network
		for (auto [input, target] : dataset.get_data(1.0)) {
			auto output = nn.get_prediction(&input);
			std::cout << "Prediction : " << output[0] << ", Target : " << target[0] << '\n';
		}

		auto [in, out] = nn.convert_data_for_loss(dataset);
		std::cout << "Total loss : " << nn.get_loss_mrse(in, out) << '\n';
	}
	{
		// Creating neural network
		std::vector<size_t> topology = {
			MAX_TOKENS_NN,
			15000,
			5000,
			1,
		};
		NeuralNetwork nn(topology, 0.01);
		std::cout << "Neural network created" << '\n';

		Dataset dataset = Dataset(new BinaryParser(), "dataset/bench_bins_small", topology);
		std::cout << "Dataset loaded" << '\n';

		int nb_epochs = 1000;
		std::cout << "Training the neural network" << '\n';

		nn.train(dataset, nb_epochs, 0.75, "training_results/binary_training.log");

		std::cout << "Training complete" << '\n';

		// Testing the neural network
		for (auto [input, target] : dataset.get_data(1.0)) {
			auto output = nn.get_prediction(&input);
			std::cout << "Prediction : " << output[0] << ", Target : " << target[0] << '\n';
		}

		auto [in, out] = nn.convert_data_for_loss(dataset);
		std::cout << "Total loss : " << nn.get_loss_mrse(in, out) << '\n';
	}
}