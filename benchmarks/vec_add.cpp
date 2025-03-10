/**
 * @file benchmarks/vec_add.cpp
 * @brief Benchmark for vector addition using SIMD instructions
 */

#define ANKERL_NANOBENCH_IMPLEMENT ///< Needed to use nanobench
#include "../nanobench/src/include/nanobench.h"
#include <immintrin.h>

#define STRINGIFY1(s) #s			///< Helper macro to stringify a value
#define STRINGIFY2(s) STRINGIFY1(s) ///< Helper macro to stringify a value

/**
 * @brief Add a label before and after the code to be able to find it in the assembly
 */
#define LABEL(name, code)                                                                                                                  \
	asm volatile(STRINGIFY2(name##_start:) : : : "memory");                                                                                \
	code;                                                                                                                                  \
	asm volatile(STRINGIFY2(name##_end:) : : : "memory");

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
void vec_add_f32_no_simd() {
	LABEL(vec_add_f32_no_simd,
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
					   : "xmm0", "xmm1", "rcx"));
}

/**
 * @brief Add two arrays of single precision floating point numbers using SIMD instructions with 128 bits registers
 */
void vec_add_f32_simd128() {
	LABEL(vec_add_f32_simd128,
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
					   : "xmm0", "xmm1", "rcx"));
}

/**
 * @brief Add two arrays of single precision floating point numbers using SIMD instructions with 256 bits registers
 */
void vec_add_f32_simd256() {
	LABEL(vec_add_f32_simd256,
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
					   : "ymm0", "ymm1", "rcx"));
}

/**
 * @brief Add two arrays of double precision floating point numbers without SIMD instructions
 */
void vec_add_f64_no_simd() {
	LABEL(vec_add_f64_no_simd,
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
					   : "xmm0", "xmm1", "rcx"));
}

/**
 * @brief Add two arrays of double precision floating point numbers using SIMD instructions with 128 bits registers
 */
void vec_add_f64_simd128() {
	LABEL(vec_add_f64_simd128,
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
					   : "xmm0", "xmm1", "rcx"));
}

/**
 * @brief Add two arrays of double precision floating point numbers using SIMD instructions with 256 bits registers
 */
void vec_add_f64_simd256() {
	LABEL(vec_add_f64_simd256,
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
					   : "ymm0", "ymm1", "rcx"));
}

/**
 * @brief Run the benchmarks for vector addition
 */
int main() {
	initialize_arrays();
	std::ostringstream oss;
	auto result = ankerl::nanobench::Bench()
					  .minEpochIterations(1000)
					  .performanceCounters(true)
					  .output(&oss)
					  .run("vec_add_f32_no_simd", vec_add_f32_no_simd)
					  .run("vec_add_f32_simd128", vec_add_f32_simd128)
					  .run("vec_add_f32_simd256", vec_add_f32_simd256)
					  .run("vec_add_f64_no_simd", vec_add_f64_no_simd)
					  .run("vec_add_f64_simd128", vec_add_f64_simd128)
					  .run("vec_add_f64_simd256", vec_add_f64_simd256)
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