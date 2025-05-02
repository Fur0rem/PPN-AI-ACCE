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

	std::vector<size_t> sizes = {100,  200,	 300,  400,	 500,  600,	 700,  800,	 900,  1000,
								 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000};

	// Compile the shader
	int ret = system("glslc ./src/culkan/gemm.comp -o ./build/gemm.spv");
	if (ret != 0) {
		std::cerr << "Error compiling shader" << std::endl;
		return -1;
	}

	Eigen::setNbThreads(6);

	for (auto size : sizes) {

		Eigen::MatrixXf a(size, size);
		Eigen::MatrixXf b(size, size);
		std::ostringstream oss;
		auto result = ankerl::nanobench::Bench()
						  .epochs(1)
						  .performanceCounters(true)
						  .output(&oss)
						  .run("CPU",
							   [&]() {
								   Eigen::MatrixXf c = a * b;
							   })
						  .run("GPU",
							   [&]() {
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

								   culkanWriteBinding(culkan, 0, &size);
								   culkanWriteBinding(culkan, 1, &size);
								   culkanWriteBinding(culkan, 2, &size);
								   culkanWriteBinding(culkan, 3, a.data());
								   culkanWriteBinding(culkan, 4, b.data());
								   culkanWriteBinding(culkan, 5, a.data());
								   culkanSetup(culkan, (size + 31) / 32, (size + 31) / 32, 1);
								   culkanRun(culkan);
								   Eigen::MatrixXf c(size, size);
								   culkanReadBinding(culkan, 5, c.data());
								   culkanDestroy(culkan);
							   })

						  .results();
		// Print oss
		// std::cout << oss.str() << '\n';

		std::cout << "Size: " << size << "x" << size << '\n';
		for (auto const& res : result) {
			auto measure = res.fromString("elapsed");
			auto name = res.config().mBenchmarkName;
			std::cout << name << ", Time: " << res.average(measure) << "s\n";
		}
		std::cout << "----------------------------\n";
	}
}