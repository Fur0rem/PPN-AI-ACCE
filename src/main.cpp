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
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <eigen3/Eigen/Dense>

void log_useless_data(Dataset* dataset) {
	Eigen::MatrixXf kernels_data = dataset->get_input_data();
	std::fstream file_proportion;
	std::fstream file_raw;
	file_proportion.open("kernel_zeros_proportion.txt", std::fstream::out);
	file_raw.open("kernel_zeros_raw.txt", std::fstream::out);

	file_raw << kernels_data.cols() << "\n";

	int count = 0;
	for (int i = 0; i < kernels_data.rows(); i++) {
		for (int j = 0; j < kernels_data.cols(); j++) {
			if (kernels_data(i, j) == -1) {
				count++;
			}
		}
		file_proportion << (double)count / (double)kernels_data.cols() << "\n";
		file_raw << count << "\n";
		count = 0;
	}

	file_proportion.close();
	file_raw.close();
}

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

	NeuralNetwork nn(topology, std::make_unique<Sigmoid>(), std::make_unique<TrainingNoise>(0.0F, 0.0F, 0.0F, 0.0F));
	std::unique_ptr<IOptimiser> optimiser = std::make_unique<Adam>(nn, 0.001F, 0.9F, 0.999F, 1e-8F);
	nn.train_batch(dataset, 2000, 0.8, 16, *optimiser, "training_results/XXX", 3);

	exit(EXIT_SUCCESS);
}