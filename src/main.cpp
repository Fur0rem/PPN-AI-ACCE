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

	NeuralNetwork nn(topology, std::make_unique<Sigmoid>());
	std::unique_ptr<IOptimiser> optimiser = std::make_unique<Adam>(nn, 0.9, 0.999, 1e-8, 0.001);
	auto dropout_rates = {0.0005F, 0.001F, 0.002F, 0.005F, 0.01F, 0.02F, 0.05F, 0.1F, 0.2F};
	for (auto dropout_rate : dropout_rates) {
		nn.train_batch(dataset,
					   100,
					   0.8,
					   16,
					   *optimiser,
					   std::string("training_results/dropout_rate_") + std::to_string(dropout_rate),
					   1,
					   dropout_rate);
	}
}