/**
 * @file benchmarks/training.cpp
 * @brief Benchmark for training the neural network.
 */

#include "neural_network/neural_network.hpp"
#include "parsing/cycles_encoders.hpp"
#include "parsing/hexadecimal_parser.hpp"
#include "parsing/size_encoder.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#define ANKERL_NANOBENCH_IMPLEMENT ///< Needed to use nanobench
#include "../nanobench/src/include/nanobench.h"

int main() {
	// Topology of the neural network
	std::vector<size_t> topology = {
		MAX_TOKENS_NN / 8,
		988,
		236,
		1047,
		70,
		1,
	};

	Dataset dataset = Dataset(new HexadecimalParser(),
							  "dataset/bench_bins",
							  topology,
							  std::make_unique<SizeEncoder>(topology[0]),
							  std::make_unique<CyclesLogEncoder>(2, 0));

	NeuralNetwork nn(topology, std::make_unique<Sigmoid>(), std::make_unique<TrainingNoise>(0.0F, 0.0F, 0.0F, 0.0F));
	std::unique_ptr<IOptimiser> optimiser = std::make_unique<Adam>(nn, 0.001F, 0.9F, 0.999F, 1e-8F);

	std::ostringstream oss;
	auto result = ankerl::nanobench::Bench()
					  .epochs(2)
					  .performanceCounters(true)
					  .output(&oss)
					  .run("Train",
						   [&]() {
							   nn.train_batch_for_topology_finder(dataset, 20, 0.8, 16, *optimiser, 0.0F, 0.0F);
						   })
					  .results();
	// Print oss
	// std::cout << oss.str() << '\n';

	for (auto const& res : result) {
		auto measure = res.fromString("elapsed");
		auto name = res.config().mBenchmarkName;
		std::cout << name << ", Min: " << res.minimum(measure) << "s, Max: " << res.maximum(measure) << "s, Med: " << res.median(measure)
				  << "s\n";
	}
	std::cout << "----------------------------\n";
}