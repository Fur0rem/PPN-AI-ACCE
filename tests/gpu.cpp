/**
 * @file tests/gpu.cpp
 * @brief Tests the GPU version of matrix multiplication
 */

#include "culkan/culkan.hpp"

#include <eigen3/Eigen/Dense>
#include <gtest/gtest.h>

TEST(GPU, GPUMatrixMultiplication) {

	size_t m = 3;
	size_t n = 3;
	size_t k = 3;

	// Generate the random matrix Eigen
	Eigen::MatrixXf a = Eigen::MatrixXf::Random(m, k);
	Eigen::MatrixXf b = Eigen::MatrixXf::Random(k, n);

	CulkanBinding bindings[] = {
		// Binding for n
		{.size = sizeof(int), .type = UNIFORM_BUFFER},
		// Binding for m
		{.size = sizeof(int), .type = UNIFORM_BUFFER},
		// Binding for k
		{.size = sizeof(int), .type = UNIFORM_BUFFER},
		// Binding for A
		{.size = m * k * sizeof(float), .type = STORAGE_BUFFER},
		// Binding for B
		{.size = k * n * sizeof(float), .type = STORAGE_BUFFER},
		// Binding for C
		{.size = m * n * sizeof(float), .type = STORAGE_BUFFER},
	};
	CulkanLayout layout = {
		.bindingCount = sizeof(bindings) / sizeof(CulkanBinding),
		.bindings = bindings,
	};

	// Compile the shader
	int ret = system("glslc ./src/culkan/gemm.comp -o ./build/gemm.spv");
	EXPECT_EQ(ret, 0);

	Eigen::MatrixXf c = Eigen::MatrixXf::Zero(m, n);

	Culkan* culkan = culkanInit(&layout, "./build/gemm.spv", (CulkanInvocations){32, 32, 1});
	culkanWriteBinding(culkan, 0, &n);
	culkanWriteBinding(culkan, 1, &m);
	culkanWriteBinding(culkan, 2, &k);
	culkanWriteBinding(culkan, 3, a.data());
	culkanWriteBinding(culkan, 4, b.data());
	culkanWriteBinding(culkan, 5, c.data());

	culkanSetup(culkan, (m + 31) / 32, (n + 31) / 32, 1);

	// Do the GPU computation
	culkanRun(culkan);
	float* result = (float*)malloc(m * n * sizeof(float));
	culkanReadBinding(culkan, 5, result);

	// Check the result
	auto result_eigen = a * b;
	for (size_t i = 0; i < m; i++) {
		for (size_t j = 0; j < n; j++) {
			EXPECT_NEAR(result[i * n + j], result_eigen(i, j), 1e-5);
		}
	}

	free(result);
	// Free the GPU memory
	culkanDestroy(culkan);
}