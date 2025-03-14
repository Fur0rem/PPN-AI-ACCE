/**
 * @file benchmarks/array_add.cpp
 * @brief Benchmark for array addition using SIMD instructions
 */

#include "bench_common.hpp"

constexpr int N = 1024;		 ///< Size of the arrays
alignas(256) float f32a[N];	 ///< Array of single precision floating point numbers
alignas(256) float f32b[N];	 ///< Array of single precision floating point numbers
alignas(256) float f32c[N];	 ///< Array of single precision floating point numbers
alignas(256) double f64a[N]; ///< Array of double precision floating point numbers
alignas(256) double f64b[N]; ///< Array of double precision floating point numbers
alignas(256) double f64c[N]; ///< Array of double precision floating point numbers

/**
 * @brief Initialize the arrays
 */
void initialize_arrays() {
	for (int i = 0; i < N; ++i) {
		f32a[i] = 1.0f;
		f32b[i] = 2.0f;
		f32c[i] = 0.0f;
		f64a[i] = 1.0;
		f64b[i] = 2.0;
		f64c[i] = 0.0;
	}
}

/**
 * @brief Add two arrays of single precision floating point numbers without SIMD instructions
 */
BENCHMARK_FN(array_add_f32_no_simd, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "movss (%0, %%rcx, 4), %%xmm0\n"
				 "movss (%1, %%rcx, 4), %%xmm1\n"
				 "addss %%xmm1, %%xmm0\n"
				 "movss %%xmm0, (%2, %%rcx, 4)\n"
				 "addq $1, %%rcx\n"
				 "cmpq %3, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(f32a), "r"(f32b), "r"(f32c), "r"((long long)N)
				 : "xmm0", "xmm1", "rcx");
})

/**
 * @brief Add two arrays of single precision floating point numbers using SIMD instructions with 128 bits registers
 */
BENCHMARK_FN(array_add_f32_simd128, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "movaps (%0, %%rcx, 4), %%xmm0\n"
				 "movaps (%1, %%rcx, 4), %%xmm1\n"
				 "addps %%xmm1, %%xmm0\n"
				 "movaps %%xmm0, (%2, %%rcx, 4)\n"
				 "addq $4, %%rcx\n"
				 "cmpq %3, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(f32a), "r"(f32b), "r"(f32c), "r"((long long)N)
				 : "xmm0", "xmm1", "rcx");
})

/**
 * @brief Add two arrays of single precision floating point numbers using SIMD instructions with 256 bits registers
 */
BENCHMARK_FN(array_add_f32_simd256, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "vmovaps (%0, %%rcx, 4), %%ymm0\n"
				 "vmovaps (%1, %%rcx, 4), %%ymm1\n"
				 "vaddps %%ymm1, %%ymm0, %%ymm0\n"
				 "vmovaps %%ymm0, (%2, %%rcx, 4)\n"
				 "addq $8, %%rcx\n"
				 "cmpq %3, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(f32a), "r"(f32b), "r"(f32c), "r"((long long)N)
				 : "ymm0", "ymm1", "rcx");
})

/**
 * @brief Add two arrays of double precision floating point numbers without SIMD instructions
 */
BENCHMARK_FN(array_add_f64_no_simd, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "movsd (%0, %%rcx, 8), %%xmm0\n"
				 "movsd (%1, %%rcx, 8), %%xmm1\n"
				 "addsd %%xmm1, %%xmm0\n"
				 "movsd %%xmm0, (%2, %%rcx, 8)\n"
				 "addq $1, %%rcx\n"
				 "cmpq %3, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(f64a), "r"(f64b), "r"(f64c), "r"((long long)N)
				 : "xmm0", "xmm1", "rcx");
})

/**
 * @brief Add two arrays of double precision floating point numbers using SIMD instructions with 128 bits registers
 */
BENCHMARK_FN(array_add_f64_simd128, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "movapd (%0, %%rcx, 8), %%xmm0\n"
				 "movapd (%1, %%rcx, 8), %%xmm1\n"
				 "addpd %%xmm1, %%xmm0\n"
				 "movapd %%xmm0, (%2, %%rcx, 8)\n"
				 "addq $2, %%rcx\n"
				 "cmpq %3, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(f64a), "r"(f64b), "r"(f64c), "r"((long long)N)
				 : "xmm0", "xmm1", "rcx");
})

/**
 * @brief Add two arrays of double precision floating point numbers using SIMD instructions with 256 bits registers
 */
BENCHMARK_FN(array_add_f64_simd256, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "vmovapd (%0, %%rcx, 8), %%ymm0\n"
				 "vmovapd (%1, %%rcx, 8), %%ymm1\n"
				 "vaddpd %%ymm1, %%ymm0, %%ymm0\n"
				 "vmovapd %%ymm0, (%2, %%rcx, 8)\n"
				 "addq $4, %%rcx\n"
				 "cmpq %3, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(f64a), "r"(f64b), "r"(f64c), "r"((long long)N)
				 : "ymm0", "ymm1", "rcx");
})

/**
 * @brief Run the benchmarks for vector addition
 */
int main() {
	initialize_arrays();
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(1000, oss)
					  .RUN(array_add_f32_no_simd)
					  .RUN(array_add_f32_simd128)
					  .RUN(array_add_f32_simd256)
					  .RUN(array_add_f64_no_simd)
					  .RUN(array_add_f64_simd128)
					  .RUN(array_add_f64_simd256)
					  .doNotOptimizeAway(f32a)
					  .doNotOptimizeAway(f32b)
					  .doNotOptimizeAway(f32c)
					  .doNotOptimizeAway(f64a)
					  .doNotOptimizeAway(f64b)
					  .results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
