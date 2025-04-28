#include "dataset/dataset.hpp"
#include "neural_network/activation_functions.hpp"
#include "neural_network/neural_network.hpp"
#include "neural_network/optimiser.hpp"
#include "parsing/cycles_encoders.hpp"
#include "parsing/hexadecimal_parser.hpp"
#include "parsing/iencoder.hpp"
#include "parsing/size_encoder.hpp"
#include <cstdlib>
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

	NeuralNetwork nn(topology, std::make_unique<Sigmoid>(), std::make_unique<TrainingNoise>(0.0F, 0.001F, 0.1F, 0.0F));
	nn.train_simulated_annealing(dataset, 200, 0.8, 0.993F, 32, 3.0F, std::string("training_results/simulated_annealing"), 1);
	nn.train_local_search(dataset, 200, 0.8, 32, std::string("training_results/local_search"), 1);
	IOptimiser* optimiser = new AMSGrad(nn, 0.3, 0.7, 1e-8, 0.0003);
	nn.train_simulated_annealing_and_gradient(
		dataset, 200, 0.8, 0.993F, 32, 3.0F, *optimiser, 20, 4, std::string("training_results/simulated_annealing_and_gradient"), 1);
	delete optimiser;

	exit(EXIT_SUCCESS);
}