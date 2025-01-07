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

constexpr int N = 1024;					   ///< Size of the arrays
alignas(256) volatile uint8_t u8_src[N];   ///< Array of unsigned 8-bit integers to be copied
alignas(256) volatile uint8_t u8_dst[N];   ///< Array of unsigned 8-bit integers to be copied to
alignas(256) volatile uint16_t u16_src[N]; ///< Array of unsigned 16-bit integers to be copied
alignas(256) volatile uint16_t u16_dst[N]; ///< Array of unsigned 16-bit integers to be copied to
alignas(256) volatile uint32_t u32_src[N]; ///< Array of unsigned 32-bit integers to be copied
alignas(256) volatile uint32_t u32_dst[N]; ///< Array of unsigned 32-bit integers to be copied to
alignas(256) volatile uint64_t u64_src[N]; ///< Array of unsigned 64-bit integers to be copied
alignas(256) volatile uint64_t u64_dst[N]; ///< Array of unsigned 64-bit integers to be copied to
alignas(256) volatile float f32_src[N];	   ///< Array of single precision floating point numbers to be copied
alignas(256) volatile float f32_dst[N];	   ///< Array of single precision floating point numbers to be copied to
alignas(256) volatile double f64_src[N];   ///< Array of double precision floating point numbers to be copied
alignas(256) volatile double f64_dst[N];   ///< Array of double precision floating point numbers to be copied to

/**
 * @brief Initialize the arrays
 */
void initialize_arrays() {
	for (int i = 0; i < N; ++i) {
		u8_src[i] = 1;
		u8_dst[i] = 0;
		u16_src[i] = 1;
		u16_dst[i] = 0;
		u32_src[i] = 1;
		u32_dst[i] = 0;
		u64_src[i] = 1;
		u64_dst[i] = 0;
	}
}

/**
 * @brief Copy an array of unsigned 8-bit integers without SIMD instructions
 */
void u8_copy_no_simd() {
	LABEL(u8_copy_no_simd,
		  asm volatile("movq $0, %%rcx\n"
					   "1:\n"
					   "movb (%0, %%rcx, 1), %%al\n"
					   "movb %%al, (%1, %%rcx, 1)\n"
					   "addq $1, %%rcx\n"
					   "cmpq %2, %%rcx\n"
					   "jl 1b\n"
					   :
					   : "r"(u8_src), "r"(u8_dst), "r"((long long)N)
					   : "al", "rcx"));
}

/**
 * @brief Copy an array of unsigned 8-bit integers using SIMD instructions with 128 bits registers
 */
void u8_copy_simd128() {
	LABEL(u8_copy_simd128,

		  asm volatile("movq $0, %%rcx\n"
					   "1:\n"
					   "movdqu (%0, %%rcx, 1), %%xmm0\n"
					   "movdqu %%xmm0, (%1, %%rcx, 1)\n"
					   "addq $16, %%rcx\n"
					   "cmpq %2, %%rcx\n"
					   "jl 1b\n"
					   :
					   : "r"(u8_src), "r"(u8_dst), "r"((long long)N)
					   : "xmm0", "rcx"));
}

/**
 * @brief Copy an array of unsigned 8-bit integers using SIMD instructions with 256 bits registers
 */
void u8_copy_simd256() {
	LABEL(u8_copy_simd256,

		  asm volatile("movq $0, %%rcx\n"
					   "1:\n"
					   "vmovdqu (%0, %%rcx, 1), %%ymm0\n"
					   "vmovdqu %%ymm0, (%1, %%rcx, 1)\n"
					   "addq $32, %%rcx\n"
					   "cmpq %2, %%rcx\n"
					   "jl 1b\n"
					   :
					   : "r"(u8_src), "r"(u8_dst), "r"((long long)N)
					   : "ymm0", "rcx"));
}

/**
 * @brief Copy an array of unsigned 16-bit integers without SIMD instructions
 */
void u16_copy_no_simd() {
	LABEL(u16_copy_no_simd,
		  asm volatile("movq $0, %%rcx\n"
					   "1:\n"
					   "movw (%0, %%rcx, 2), %%ax\n"
					   "movw %%ax, (%1, %%rcx, 2)\n"
					   "addq $2, %%rcx\n"
					   "cmpq %2, %%rcx\n"
					   "jl 1b\n"
					   :
					   : "r"(u16_src), "r"(u16_dst), "r"((long long)N)
					   : "ax", "rcx"));
}

/**
 * @brief Copy an array of unsigned 16-bit integers using SIMD instructions with 128 bits registers
 */
void u16_copy_simd128() {
	LABEL(u16_copy_simd128,

		  asm volatile("movq $0, %%rcx\n"
					   "1:\n"
					   "movdqu (%0, %%rcx, 2), %%xmm0\n"
					   "movdqu %%xmm0, (%1, %%rcx, 2)\n"
					   "addq $16, %%rcx\n"
					   "cmpq %2, %%rcx\n"
					   "jl 1b\n"
					   :
					   : "r"(u16_src), "r"(u16_dst), "r"((long long)N)
					   : "xmm0", "rcx"));
}

/**
 * @brief Copy an array of unsigned 16-bit integers using SIMD instructions with 256 bits registers
 */
void u16_copy_simd256() {
	LABEL(u16_copy_simd256,

		  asm volatile("movq $0, %%rcx\n"
					   "1:\n"
					   "vmovdqu (%0, %%rcx, 2), %%ymm0\n"
					   "vmovdqu %%ymm0, (%1, %%rcx, 2)\n"
					   "addq $32, %%rcx\n"
					   "cmpq %2, %%rcx\n"
					   "jl 1b\n"
					   :
					   : "r"(u16_src), "r"(u16_dst), "r"((long long)N)
					   : "ymm0", "rcx"));
}

/**
 * @brief Copy an array of unsigned 32-bit integers without SIMD instructions
 */
void u32_copy_no_simd() {
	LABEL(u32_copy_no_simd,
		  asm volatile("movq $0, %%rcx\n"
					   "1:\n"
					   "movl (%0, %%rcx, 4), %%eax\n"
					   "movl %%eax, (%1, %%rcx, 4)\n"
					   "addq $4, %%rcx\n"
					   "cmpq %2, %%rcx\n"
					   "jl 1b\n"
					   :
					   : "r"(u32_src), "r"(u32_dst), "r"((long long)N)
					   : "eax", "rcx"));
}

/**
 * @brief Copy an array of unsigned 32-bit integers using SIMD instructions with 128 bits registers
 */
void u32_copy_simd128() {
	LABEL(u32_copy_simd128,

		  asm volatile("movq $0, %%rcx\n"
					   "1:\n"
					   "movdqa (%0, %%rcx, 4), %%xmm0\n"
					   "movdqa %%xmm0, (%1, %%rcx, 4)\n"
					   "addq $16, %%rcx\n"
					   "cmpq %2, %%rcx\n"
					   "jl 1b\n"
					   :
					   : "r"(u32_src), "r"(u32_dst), "r"((long long)N)
					   : "xmm0", "rcx"));
}

/**
 * @brief Copy an array of unsigned 32-bit integers using SIMD instructions with 256 bits registers
 */
void u32_copy_simd256() {
	LABEL(u32_copy_simd256,

		  asm volatile("movq $0, %%rcx\n"
					   "1:\n"
					   "vmovdqa (%0, %%rcx, 4), %%ymm0\n"
					   "vmovdqa %%ymm0, (%1, %%rcx, 4)\n"
					   "addq $32, %%rcx\n"
					   "cmpq %2, %%rcx\n"
					   "jl 1b\n"
					   :
					   : "r"(u32_src), "r"(u32_dst), "r"((long long)N)
					   : "ymm0", "rcx"));
}

/**
 * @brief Copy an array of unsigned 64-bit integers without SIMD instructions
 */
void u64_copy_no_simd() {
	LABEL(u64_copy_no_simd,
		  asm volatile("movq $0, %%rcx\n"
					   "1:\n"
					   "movq (%0, %%rcx, 8), %%rax\n"
					   "movq %%rax, (%1, %%rcx, 8)\n"
					   "addq $8, %%rcx\n"
					   "cmpq %2, %%rcx\n"
					   "jl 1b\n"
					   :
					   : "r"(u64_src), "r"(u64_dst), "r"((long long)N)
					   : "rax", "rcx"));
}

/**
 * @brief Copy an array of unsigned 64-bit integers using SIMD instructions with 128 bits registers
 */
void u64_copy_simd128() {
	LABEL(u64_copy_simd128,
		  asm volatile("movq $0, %%rcx\n"
					   "1:\n"
					   "movdqa (%0, %%rcx, 8), %%xmm0\n"
					   "movdqa %%xmm0, (%1, %%rcx, 8)\n"
					   "addq $16, %%rcx\n"
					   "cmpq %2, %%rcx\n"
					   "jl 1b\n"
					   :
					   : "r"(u64_src), "r"(u64_dst), "r"((long long)N)
					   : "xmm0", "rcx"));
}

/**
 * @brief Copy an array of unsigned 64-bit integers using SIMD instructions with 256 bits registers
 */
void u64_copy_simd256() {
	LABEL(u64_copy_simd256,

		  asm volatile("movq $0, %%rcx\n"
					   "1:\n"
					   "vmovdqa (%0, %%rcx, 8), %%ymm0\n"
					   "vmovdqa %%ymm0, (%1, %%rcx, 8)\n"
					   "addq $32, %%rcx\n"
					   "cmpq %2, %%rcx\n"
					   "jl 1b\n"
					   :
					   : "r"(u64_src), "r"(u64_dst), "r"((long long)N)
					   : "ymm0", "rcx"));
}

/**
 * @brief Copy an array of single precision floating point numbers without SIMD instructions
 */
void f32_copy_no_simd() {
	LABEL(f32_copy_no_simd,
		  asm volatile("movq $0, %%rcx\n"
					   "1:\n"
					   "movss (%0, %%rcx, 4), %%xmm0\n"
					   "movss %%xmm0, (%1, %%rcx, 4)\n"
					   "addq $4, %%rcx\n"
					   "cmpq %2, %%rcx\n"
					   "jl 1b\n"
					   :
					   : "r"(f32_src), "r"(f32_dst), "r"((long long)N)
					   : "xmm0", "rcx"));
}

/**
 * @brief Copy an array of single precision floating point numbers using SIMD instructions with 128 bits registers
 */
void f32_copy_simd128() {
	LABEL(f32_copy_simd128,
		  asm volatile("movq $0, %%rcx\n"
					   "1:\n"
					   "movaps (%0, %%rcx, 4), %%xmm0\n"
					   "movaps %%xmm0, (%1, %%rcx, 4)\n"
					   "addq $16, %%rcx\n"
					   "cmpq %2, %%rcx\n"
					   "jl 1b\n"
					   :
					   : "r"(f32_src), "r"(f32_dst), "r"((long long)N)
					   : "xmm0", "rcx"));
}

/**
 * @brief Copy an array of single precision floating point numbers using SIMD instructions with 256 bits registers
 */
void f32_copy_simd256() {
	LABEL(f32_copy_simd256,
		  asm volatile("movq $0, %%rcx\n"
					   "1:\n"
					   "vmovaps (%0, %%rcx, 4), %%ymm0\n"
					   "vmovaps %%ymm0, (%1, %%rcx, 4)\n"
					   "addq $32, %%rcx\n"
					   "cmpq %2, %%rcx\n"
					   "jl 1b\n"
					   :
					   : "r"(f32_src), "r"(f32_dst), "r"((long long)N)
					   : "ymm0", "rcx"));
}

/**
 * @brief Copy an array of double precision floating point numbers without SIMD instructions
 */
void f64_copy_no_simd() {
	LABEL(f64_copy_no_simd,
		  asm volatile("movq $0, %%rcx\n"
					   "1:\n"
					   "movsd (%0, %%rcx, 8), %%xmm0\n"
					   "movsd %%xmm0, (%1, %%rcx, 8)\n"
					   "addq $8, %%rcx\n"
					   "cmpq %2, %%rcx\n"
					   "jl 1b\n"
					   :
					   : "r"(f64_src), "r"(f64_dst), "r"((long long)N)
					   : "xmm0", "rcx"));
}

/**
 * @brief Copy an array of double precision floating point numbers using SIMD instructions with 128 bits registers
 */
void f64_copy_simd128() {
	LABEL(f64_copy_simd128,
		  asm volatile("movq $0, %%rcx\n"
					   "1:\n"
					   "movapd (%0, %%rcx, 8), %%xmm0\n"
					   "movapd %%xmm0, (%1, %%rcx, 8)\n"
					   "addq $16, %%rcx\n"
					   "cmpq %2, %%rcx\n"
					   "jl 1b\n"
					   :
					   : "r"(f64_src), "r"(f64_dst), "r"((long long)N)
					   : "xmm0", "rcx"));
}

/**
 * @brief Copy an array of double precision floating point numbers using SIMD instructions with 256 bits registers
 */
void f64_copy_simd256() {
	LABEL(f64_copy_simd256,
		  asm volatile("movq $0, %%rcx\n"
					   "1:\n"
					   "vmovapd (%0, %%rcx, 8), %%ymm0\n"
					   "vmovapd %%ymm0, (%1, %%rcx, 8)\n"
					   "addq $32, %%rcx\n"
					   "cmpq %2, %%rcx\n"
					   "jl 1b\n"
					   :
					   : "r"(f64_src), "r"(f64_dst), "r"((long long)N)
					   : "ymm0", "rcx"));
}

/**
 * @brief Run the benchmarks for data copy
 */
int main() {
	initialize_arrays();
	std::ostringstream oss;
	auto result = ankerl::nanobench::Bench()
					  .minEpochIterations(1000)
					  .performanceCounters(true)
					  .output(&oss)
					  .run("u8_copy_no_simd", u8_copy_no_simd)
					  .run("u8_copy_simd128", u8_copy_simd128)
					  .run("u8_copy_simd256", u8_copy_simd256)
					  .run("u16_copy_no_simd", u16_copy_no_simd)
					  .run("u16_copy_simd128", u16_copy_simd128)
					  .run("u16_copy_simd256", u16_copy_simd256)
					  .run("u32_copy_no_simd", u32_copy_no_simd)
					  .run("u32_copy_simd128", u32_copy_simd128)
					  .run("u32_copy_simd256", u32_copy_simd256)
					  .run("u64_copy_no_simd", u64_copy_no_simd)
					  .run("u64_copy_simd128", u64_copy_simd128)
					  .run("u64_copy_simd256", u64_copy_simd256)
					  .run("f32_copy_no_simd", f32_copy_no_simd)
					  .run("f32_copy_simd128", f32_copy_simd128)
					  .run("f32_copy_simd256", f32_copy_simd256)
					  .run("f64_copy_no_simd", f64_copy_no_simd)
					  .run("f64_copy_simd128", f64_copy_simd128)
					  .run("f64_copy_simd256", f64_copy_simd256)
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