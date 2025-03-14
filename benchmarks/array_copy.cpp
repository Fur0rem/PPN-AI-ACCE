/**
 * @file benchmarks/array_copy.cpp
 * @brief Benchmark for copying arrays with and without SIMD instructions
 */

#include "bench_common.hpp"

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
BENCHMARK_FN(u8_copy_no_simd, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "movb (%0, %%rcx, 1), %%al\n"
				 "movb %%al, (%1, %%rcx, 1)\n"
				 "addq $1, %%rcx\n"
				 "cmpq %2, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(u8_src), "r"(u8_dst), "r"((long long)N)
				 : "al", "rcx");
})

/**
 * @brief Copy an array of unsigned 8-bit integers using SIMD instructions with 128 bits registers
 */
BENCHMARK_FN(u8_copy_simd128, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "movdqu (%0, %%rcx, 1), %%xmm0\n"
				 "movdqu %%xmm0, (%1, %%rcx, 1)\n"
				 "addq $16, %%rcx\n"
				 "cmpq %2, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(u8_src), "r"(u8_dst), "r"((long long)N)
				 : "xmm0", "rcx");
})
/**
 * @brief Copy an array of unsigned 8-bit integers using SIMD instructions with 256 bits registers
 */
BENCHMARK_FN(u8_copy_simd256, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "vmovdqu (%0, %%rcx, 1), %%ymm0\n"
				 "vmovdqu %%ymm0, (%1, %%rcx, 1)\n"
				 "addq $32, %%rcx\n"
				 "cmpq %2, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(u8_src), "r"(u8_dst), "r"((long long)N)
				 : "ymm0", "rcx");
})

/**
 * @brief Copy an array of unsigned 16-bit integers without SIMD instructions
 */
BENCHMARK_FN(u16_copy_no_simd, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "movw (%0, %%rcx, 2), %%ax\n"
				 "movw %%ax, (%1, %%rcx, 2)\n"
				 "addq $2, %%rcx\n"
				 "cmpq %2, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(u16_src), "r"(u16_dst), "r"((long long)N)
				 : "ax", "rcx");
})

/**
 * @brief Copy an array of unsigned 16-bit integers using SIMD instructions with 128 bits registers
 */
BENCHMARK_FN(u16_copy_simd128, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "movdqu (%0, %%rcx, 2), %%xmm0\n"
				 "movdqu %%xmm0, (%1, %%rcx, 2)\n"
				 "addq $16, %%rcx\n"
				 "cmpq %2, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(u16_src), "r"(u16_dst), "r"((long long)N)
				 : "xmm0", "rcx");
})

/**
 * @brief Copy an array of unsigned 16-bit integers using SIMD instructions with 256 bits registers
 */
BENCHMARK_FN(u16_copy_simd256, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "vmovdqu (%0, %%rcx, 2), %%ymm0\n"
				 "vmovdqu %%ymm0, (%1, %%rcx, 2)\n"
				 "addq $32, %%rcx\n"
				 "cmpq %2, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(u16_src), "r"(u16_dst), "r"((long long)N)
				 : "ymm0", "rcx");
})

/**
 * @brief Copy an array of unsigned 32-bit integers without SIMD instructions
 */
BENCHMARK_FN(u32_copy_no_simd, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "movl (%0, %%rcx, 4), %%eax\n"
				 "movl %%eax, (%1, %%rcx, 4)\n"
				 "addq $4, %%rcx\n"
				 "cmpq %2, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(u32_src), "r"(u32_dst), "r"((long long)N)
				 : "eax", "rcx");
})

/**
 * @brief Copy an array of unsigned 32-bit integers using SIMD instructions with 128 bits registers
 */
BENCHMARK_FN(u32_copy_simd128, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "movdqa (%0, %%rcx, 4), %%xmm0\n"
				 "movdqa %%xmm0, (%1, %%rcx, 4)\n"
				 "addq $16, %%rcx\n"
				 "cmpq %2, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(u32_src), "r"(u32_dst), "r"((long long)N)
				 : "xmm0", "rcx");
})

/**
 * @brief Copy an array of unsigned 32-bit integers using SIMD instructions with 256 bits registers
 */
BENCHMARK_FN(u32_copy_simd256, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "vmovdqa (%0, %%rcx, 4), %%ymm0\n"
				 "vmovdqa %%ymm0, (%1, %%rcx, 4)\n"
				 "addq $32, %%rcx\n"
				 "cmpq %2, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(u32_src), "r"(u32_dst), "r"((long long)N)
				 : "ymm0", "rcx");
})

/**
 * @brief Copy an array of unsigned 64-bit integers without SIMD instructions
 */
BENCHMARK_FN(u64_copy_no_simd, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "movq (%0, %%rcx, 8), %%rax\n"
				 "movq %%rax, (%1, %%rcx, 8)\n"
				 "addq $8, %%rcx\n"
				 "cmpq %2, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(u64_src), "r"(u64_dst), "r"((long long)N)
				 : "rax", "rcx");
})

/**
 * @brief Copy an array of unsigned 64-bit integers using SIMD instructions with 128 bits registers
 */
BENCHMARK_FN(u64_copy_simd128, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "movdqa (%0, %%rcx, 8), %%xmm0\n"
				 "movdqa %%xmm0, (%1, %%rcx, 8)\n"
				 "addq $16, %%rcx\n"
				 "cmpq %2, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(u64_src), "r"(u64_dst), "r"((long long)N)
				 : "xmm0", "rcx");
})

/**
 * @brief Copy an array of unsigned 64-bit integers using SIMD instructions with 256 bits registers
 */
BENCHMARK_FN(u64_copy_simd256, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "vmovdqa (%0, %%rcx, 8), %%ymm0\n"
				 "vmovdqa %%ymm0, (%1, %%rcx, 8)\n"
				 "addq $32, %%rcx\n"
				 "cmpq %2, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(u64_src), "r"(u64_dst), "r"((long long)N)
				 : "ymm0", "rcx");
})

/**
 * @brief Copy an array of single precision floating point numbers without SIMD instructions
 */
BENCHMARK_FN(f32_copy_no_simd, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "movss (%0, %%rcx, 4), %%xmm0\n"
				 "movss %%xmm0, (%1, %%rcx, 4)\n"
				 "addq $4, %%rcx\n"
				 "cmpq %2, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(f32_src), "r"(f32_dst), "r"((long long)N)
				 : "xmm0", "rcx");
})

/**
 * @brief Copy an array of single precision floating point numbers using SIMD instructions with 128 bits registers
 */
BENCHMARK_FN(f32_copy_simd128, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "movaps (%0, %%rcx, 4), %%xmm0\n"
				 "movaps %%xmm0, (%1, %%rcx, 4)\n"
				 "addq $16, %%rcx\n"
				 "cmpq %2, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(f32_src), "r"(f32_dst), "r"((long long)N)
				 : "xmm0", "rcx");
})

/**
 * @brief Copy an array of single precision floating point numbers using SIMD instructions with 256 bits registers
 */
BENCHMARK_FN(f32_copy_simd256, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "vmovaps (%0, %%rcx, 4), %%ymm0\n"
				 "vmovaps %%ymm0, (%1, %%rcx, 4)\n"
				 "addq $32, %%rcx\n"
				 "cmpq %2, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(f32_src), "r"(f32_dst), "r"((long long)N)
				 : "ymm0", "rcx");
})

/**
 * @brief Copy an array of double precision floating point numbers without SIMD instructions
 */
BENCHMARK_FN(f64_copy_no_simd, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "movsd (%0, %%rcx, 8), %%xmm0\n"
				 "movsd %%xmm0, (%1, %%rcx, 8)\n"
				 "addq $8, %%rcx\n"
				 "cmpq %2, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(f64_src), "r"(f64_dst), "r"((long long)N)
				 : "xmm0", "rcx");
})

/**
 * @brief Copy an array of double precision floating point numbers using SIMD instructions with 128 bits registers
 */
BENCHMARK_FN(f64_copy_simd128, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "movapd (%0, %%rcx, 8), %%xmm0\n"
				 "movapd %%xmm0, (%1, %%rcx, 8)\n"
				 "addq $16, %%rcx\n"
				 "cmpq %2, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(f64_src), "r"(f64_dst), "r"((long long)N)
				 : "xmm0", "rcx");
})

/**
 * @brief Copy an array of double precision floating point numbers using SIMD instructions with 256 bits registers
 */
BENCHMARK_FN(f64_copy_simd256, {
	asm volatile("movq $0, %%rcx\n"
				 "1:\n"
				 "vmovapd (%0, %%rcx, 8), %%ymm0\n"
				 "vmovapd %%ymm0, (%1, %%rcx, 8)\n"
				 "addq $32, %%rcx\n"
				 "cmpq %2, %%rcx\n"
				 "jl 1b\n"
				 :
				 : "r"(f64_src), "r"(f64_dst), "r"((long long)N)
				 : "ymm0", "rcx");
})

/**
 * @brief Copy an array of double precision floating point numbers using Duff's device
 */
BENCHMARK_FN(f64_copy_duffs_device, {
	volatile double* src = f64_src;
	volatile double* dst = f64_dst;
	int n = N;
	int count = (n + 7) / 8;
	switch (n % 8) {
		case 0:
			do {
				*dst++ = *src++;
				case 7:
					*dst++ = *src++;
				case 6:
					*dst++ = *src++;
				case 5:
					*dst++ = *src++;
				case 4:
					*dst++ = *src++;
				case 3:
					*dst++ = *src++;
				case 2:
					*dst++ = *src++;
				case 1:
					*dst++ = *src++;
			} while (--count > 0);
	}
})

/**
 * @brief Run the benchmarks for array copy
 */
int main() {
	initialize_arrays();
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(1000, oss)
					  .RUN(u8_copy_no_simd)
					  .RUN(u8_copy_simd128)
					  .RUN(u8_copy_simd256)
					  .RUN(u16_copy_no_simd)
					  .RUN(u16_copy_simd128)
					  .RUN(u16_copy_simd256)
					  .RUN(u32_copy_no_simd)
					  .RUN(u32_copy_simd128)
					  .RUN(u32_copy_simd256)
					  .RUN(u64_copy_no_simd)
					  .RUN(u64_copy_simd128)
					  .RUN(u64_copy_simd256)
					  .RUN(f32_copy_no_simd)
					  .RUN(f32_copy_simd128)
					  .RUN(f32_copy_simd256)
					  .RUN(f64_copy_no_simd)
					  .RUN(f64_copy_simd128)
					  .RUN(f64_copy_simd256)
					  .RUN(f64_copy_duffs_device)
					  .doNotOptimizeAway(u8_src)
					  .doNotOptimizeAway(u8_dst)
					  .doNotOptimizeAway(u16_src)
					  .doNotOptimizeAway(u16_dst)
					  .doNotOptimizeAway(u32_src)
					  .doNotOptimizeAway(u32_dst)
					  .doNotOptimizeAway(u64_src)
					  .doNotOptimizeAway(u64_dst)
					  .doNotOptimizeAway(f32_src)
					  .doNotOptimizeAway(f32_dst)
					  .doNotOptimizeAway(f64_src)
					  .doNotOptimizeAway(f64_dst)
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
