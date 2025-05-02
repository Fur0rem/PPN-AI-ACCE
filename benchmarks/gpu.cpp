/**
 * @file benchmarks/gpu.cpp
 * @brief Benchmark for the GPU version of GEMM
 */

#include "culkan/culkan.hpp"

#include <eigen3/Eigen/Dense>
#include <iostream>
#include <sstream>
#include <vector>

#define ANKERL_NANOBENCH_IMPLEMENT ///< Needed to use nanobench
#include "../nanobench/src/include/nanobench.h"

int main() {
	// // Topology of the neural network
	// std::vector<size_t> topology = {
	// 	MAX_TOKENS_NN / 8,
	// 	988,
	// 	236,
	// 	1047,
	// 	70,
	// 	1,
	// };

	// Dataset dataset = Dataset(new HexadecimalParser(),
	// 						  "dataset/bench_bins",
	// 						  topology,
	// 						  std::make_unique<SizeEncoder>(topology[0]),
	// 						  std::make_unique<CyclesLogEncoder>(2, 0));

	// NeuralNetwork nn(topology, std::make_unique<Sigmoid>(), std::make_unique<TrainingNoise>(0.0F, 0.0F, 0.0F, 0.0F));
	// std::unique_ptr<IOptimiser> optimiser = std::make_unique<Adam>(nn, 0.001F, 0.9F, 0.999F, 1e-8F);

	// Eigen::setNbThreads(6);

	// std::ostringstream oss;
	// auto result = ankerl::nanobench::Bench()
	// 				  .epochs(2)
	// 				  .performanceCounters(true)
	// 				  .output(&oss)
	// 				  .run("Train",
	// 					   [&]() {
	// 						   nn.train_batch_for_topology_finder(dataset, 20, 0.8, 16, *optimiser, 0.0F, 0.0F);
	// 					   })
	// 				  .results();
	// // Print oss
	// // std::cout << oss.str() << '\n';

	// for (auto const& res : result) {
	// 	auto measure = res.fromString("elapsed");
	// 	auto name = res.config().mBenchmarkName;
	// 	std::cout << name << ", Min: " << res.minimum(measure) << "s, Max: " << res.maximum(measure) << "s, Med: " << res.median(measure)
	// 			  << "s\n";
	// }
	// std::cout << "----------------------------\n";

	std::vector<size_t> sizes = {250, 500, 1000, 2000, 4000};
	size_t max_size = 4000;

	// Compile the shader
	int ret = system("glslc ./src/culkan/gemm.comp -o ./build/gemm.spv");
	if (ret != 0) {
		std::cerr << "Error compiling shader" << std::endl;
		return -1;
	}

	Eigen::setNbThreads(6);

	for (auto size : sizes) {
		CulkanBinding bindings[] = {
			// Binding for n
			{.size = sizeof(int), .type = UNIFORM_BUFFER},
			// Binding for m
			{.size = sizeof(int), .type = UNIFORM_BUFFER},
			// Binding for k
			{.size = sizeof(int), .type = UNIFORM_BUFFER},
			// Binding for A
			{.size = size * size * sizeof(float), .type = STORAGE_BUFFER},
			// Binding for B
			{.size = size * size * sizeof(float), .type = STORAGE_BUFFER},
			// Binding for C
			{.size = size * size * sizeof(float), .type = STORAGE_BUFFER},
		};
		CulkanLayout layout = {
			.bindingCount = sizeof(bindings) / sizeof(CulkanBinding),
			.bindings = bindings,
		};

		Culkan* culkan = culkanInit(&layout, "./build/gemm.spv", (CulkanInvocations){32, 32, 1});

		// culkanSetup(culkan, (max_size + 31) / 32, (max_size + 31) / 32, 1);

		Eigen::MatrixXf a(size, size);
		Eigen::MatrixXf b(size, size);
		std::ostringstream oss;
		auto result = ankerl::nanobench::Bench()
						  .epochs(3)
						  .performanceCounters(true)
						  .output(&oss)
						  .run("CPU",
							   [&]() {
								   Eigen::MatrixXf c = a * b;
							   })
						  .run("GPU with memory copy",
							   [&]() {
								   culkanWriteBinding(culkan, 0, &size);
								   culkanWriteBinding(culkan, 1, &size);
								   culkanWriteBinding(culkan, 2, &size);
								   culkanWriteBinding(culkan, 3, a.data());
								   culkanWriteBinding(culkan, 4, b.data());
								   culkanWriteBinding(culkan, 5, a.data());
								   culkanRun(culkan);
								   Eigen::MatrixXf c(size, size);
								   culkanReadBinding(culkan, 5, c.data());
							   })
						  .run("GPU without memory copy",
							   [&]() {
								   culkanRun(culkan);
							   })

						  .results();
		// Print oss
		// std::cout << oss.str() << '\n';

		std::cout << "Size: " << size << "x" << size << '\n';
		for (auto const& res : result) {
			auto measure = res.fromString("elapsed");
			auto name = res.config().mBenchmarkName;
			std::cout << name << ", Min: " << res.minimum(measure) << "s, Max: " << res.maximum(measure)
					  << "s, Med: " << res.median(measure) << "s\n";
		}
		std::cout << "----------------------------\n";
	}
}