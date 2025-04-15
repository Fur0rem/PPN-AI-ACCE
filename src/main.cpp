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
							  "dataset/bench_bins_small",
							  topology,
							  std::make_unique<SizeEncoder>(topology[0]),
							  std::make_unique<CyclesLogEncoder>(2, 0));

	// Create the neural network
	NeuralNetwork nn(topology, std::make_unique<Sigmoid>());

	// Train the neural network
	std::vector<float> learning_rates = {0.00005, 0.0001, 0.0002, 0.0005, 0.001, 0.002, 0.005, 0.01, 0.02, 0.05, 0.1, 0.2};
	size_t nb_epochs = 2000;

	for (auto& learning_rate : learning_rates) {
		std::vector<IOptimiser*> optimisers = {
			new AMSGrad(nn, 0.9, 0.999, 1e-8, learning_rate),
			new RMSProp(nn, 0.9, 1e-8, learning_rate),
			new Adam(nn, 0.9, 0.999, 1e-8, learning_rate),
			new SGD(learning_rate),
			new Momentum(nn, 0.8, learning_rate),
		};
		for (auto& opt : optimisers) {
			nn.train_batch(dataset,
						   nb_epochs,
						   0.8,
						   16,
						   *opt,
						   std::string("training_results/") + typeid(*opt).name() + "_" + std::to_string(learning_rate),
						   2);
		}
	}
}