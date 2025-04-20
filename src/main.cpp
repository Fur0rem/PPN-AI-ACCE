#include "dataset/dataset.hpp"
#include "neural_network/activation_functions.hpp"
#include "neural_network/neural_network.hpp"
#include "parsing/cycles_encoders.hpp"
#include "parsing/hexadecimal_parser.hpp"
#include "parsing/iencoder.hpp"
#include "parsing/size_encoder.hpp"
#include <cxxabi.h>
#include <memory>
#include <string>
#include <vector>

#include <eigen3/Eigen/Dense>

int main() {
	// Topology of the neural network
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

	std::vector<std::unique_ptr<ActivationFunc>> activation_functions;
	activation_functions.push_back(std::make_unique<Tanh>());
	activation_functions.push_back(std::make_unique<ELU>());
	activation_functions.push_back(std::make_unique<Sigmoid>());
	activation_functions.push_back(std::make_unique<ReLU>());
	activation_functions.push_back(std::make_unique<LeakyReLU>());
	activation_functions.push_back(std::make_unique<GELU>());

	std::vector<std::string> activation_function_names = {
		"Tanh",
		"Sigmoid",
		"ReLU",
		"LeakyReLU",
		"GELU",
		"ELU",
	};

	for (size_t i = 0; i < activation_functions.size(); ++i) {
		auto& activation_function = activation_functions[i];
		NeuralNetwork nn(topology, std::move(activation_function));
		nn.train(dataset, 2000, 0.8, 0.02, std::string("training_results/") + activation_function_names[i], 1, 0);
	}
}