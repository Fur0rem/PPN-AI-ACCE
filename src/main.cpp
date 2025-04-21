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

	auto add_noises = {0.000F, 0.01F, 0.1F, 0.2F};
	auto mul_noises = {0.000F, 0.01F, 0.1F, 0.2F};

	for (auto add_noise : add_noises) {
		for (auto mul_noise : mul_noises) {
			if (add_noise == 0.0F && mul_noise == 0.0F) {
				continue;
			}
			auto nn = NeuralNetwork(topology, std::make_unique<Sigmoid>(), std::make_unique<TrainingNoise>(0.0F, add_noise, mul_noise));
			std::unique_ptr<IOptimiser> optimiser = std::make_unique<Adam>(nn, 0.9, 0.999, 1e-8, 0.001);
			nn.train_batch(dataset,
						   100,
						   0.8,
						   16,
						   *optimiser,
						   std::string("training_results/noise_add_") + std::to_string(add_noise) + "_mul_" + std::to_string(mul_noise),
						   1);
		}
	}
}