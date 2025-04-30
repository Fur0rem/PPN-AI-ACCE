/**
 * @file codelets/array_min.cpp
 * @brief Benchmark for finding the minimum in an array of integers and floating point numbers
 */

#include "bench_common.hpp"

constexpr int N = 1024;						  ///< Size of the arrays
alignas(256) constexpr uint8_t U8[N] = {1};	  ///< Array of unsigned 8-bit integers to be copied
alignas(256) constexpr uint16_t U16[N] = {1}; ///< Array of unsigned 16-bit integers to be copied
alignas(256) constexpr uint32_t U32[N] = {1}; ///< Array of unsigned 32-bit integers to be copied
alignas(256) constexpr uint64_t U64[N] = {1}; ///< Array of unsigned 64-bit integers to be copied
alignas(256) constexpr float F32[N] = {1};	  ///< Array of single precision floating point numbers to be copied
alignas(256) constexpr double F64[N] = {1};	  ///< Array of double precision floating point numbers to be copied

/**
 * @brief Finds the minimum in an array of u8's without using SIMD instructions
 */
BENCHMARK_FN(u8_min_no_simd, {
	volatile uint8_t min = U8[0];
	for (int i = 1; i < N; i++) {
		if (U8[i] < min) {
			min = U8[i];
		}
	}
})

/**
 * @brief Finds the minimum in an array of u16's without using SIMD instructions
 */
BENCHMARK_FN(u16_min_no_simd, {
	volatile uint16_t min = U16[0];
	for (int i = 1; i < N; i++) {
		if (U16[i] < min) {
			min = U16[i];
		}
	}
})

/**
 * @brief Finds the minimum in an array of u32's without using SIMD instructions
 */
BENCHMARK_FN(u32_min_no_simd, {
	volatile uint32_t min = U32[0];
	for (int i = 1; i < N; i++) {
		if (U32[i] < min) {
			min = U32[i];
		}
	}
})

/**
 * @brief Finds the minimum in an array of u64's without using SIMD instructions
 */
BENCHMARK_FN(u64_min_no_simd, {
	volatile uint64_t min = U64[0];
	for (int i = 1; i < N; i++) {
		if (U64[i] < min) {
			min = U64[i];
		}
	}
})

/**
 * @brief Finds the minimum in an array of f32's without using SIMD instructions
 */
BENCHMARK_FN(f32_min_no_simd, {
	volatile float min = F32[0];
	for (int i = 1; i < N; i++) {
		if (F32[i] < min) {
			min = F32[i];
		}
	}
})

/**
 * @brief Finds the minimum in an array of f64's without using SIMD instructions
 */
BENCHMARK_FN(f64_min_no_simd, {
	volatile double min = F64[0];
	for (int i = 1; i < N; i++) {
		if (F64[i] < min) {
			min = F64[i];
		}
	}
})

/**
 * @brief Finds the minimum in an array of u8's using SIMD instructions with 128 bits registers
 */
BENCHMARK_FN(u8_min_simd128, {
	volatile uint8_t min = U8[0];
	__m128i min_v = _mm_set1_epi8(min);
	for (int i = 1; i < N; i += 16) {
		__m128i v = _mm_load_si128((__m128i*)&U8[i]);
		min_v = _mm_min_epu8(min_v, v);
	}
	min = _mm_extract_epi8(min_v, 0);
})

/**
 * @brief Finds the minimum in an array of u8's using SIMD instructions with 256 bits registers
 */
BENCHMARK_FN(u8_min_simd256, {
	volatile uint8_t min = U8[0];
	__m256i min_v = _mm256_set1_epi8(min);
	for (int i = 1; i < N; i += 32) {
		__m256i v = _mm256_load_si256((__m256i*)&U8[i]);
		min_v = _mm256_min_epu8(min_v, v);
	}
	min = _mm256_extract_epi8(min_v, 0);
})

/**
 * @brief Finds the minimum in an array of u16's using SIMD instructions with 128 bits registers
 */
BENCHMARK_FN(u16_min_simd128, {
	volatile uint16_t min = U16[0];
	__m128i min_v = _mm_set1_epi16(min);
	for (int i = 1; i < N; i += 8) {
		__m128i v = _mm_load_si128((__m128i*)&U16[i]);
		min_v = _mm_min_epu16(min_v, v);
	}
	min = _mm_extract_epi16(min_v, 0);
})

/**
 * @brief Finds the minimum in an array of u16's using SIMD instructions with 256 bits registers
 */
BENCHMARK_FN(u16_min_simd256, {
	volatile uint16_t min = U16[0];
	__m256i min_v = _mm256_set1_epi16(min);
	for (int i = 1; i < N; i += 16) {
		__m256i v = _mm256_load_si256((__m256i*)&U16[i]);
		min_v = _mm256_min_epu16(min_v, v);
	}
	min = _mm256_extract_epi16(min_v, 0);
})

/**
 * @brief Finds the minimum in an array of u32's using SIMD instructions with 128 bits registers
 */
BENCHMARK_FN(u32_min_simd128, {
	volatile uint32_t min = U32[0];
	__m128i min_v = _mm_set1_epi32(min);
	for (int i = 1; i < N; i += 4) {
		__m128i v = _mm_load_si128((__m128i*)&U32[i]);
		min_v = _mm_min_epu32(min_v, v);
	}
	min = _mm_extract_epi32(min_v, 0);
})

/**
 * @brief Finds the minimum in an array of u32's using SIMD instructions with 256 bits registers
 */
BENCHMARK_FN(u32_min_simd256, {
	volatile uint32_t min = U32[0];
	__m256i min_v = _mm256_set1_epi32(min);
	for (int i = 1; i < N; i += 8) {
		__m256i v = _mm256_load_si256((__m256i*)&U32[i]);
		min_v = _mm256_min_epu32(min_v, v);
	}
	min = _mm256_extract_epi32(min_v, 0);
})

/**
 * @brief Finds the minimum in an array of f32's using SIMD instructions with 128 bits registers
 */
BENCHMARK_FN(f32_min_simd128, {
	volatile float min = F32[0];
	__m128 min_v = _mm_set1_ps(min);
	for (int i = 1; i < N; i += 4) {
		__m128 v = _mm_load_ps(&F32[i]);
		min_v = _mm_min_ps(min_v, v);
	}
	min = _mm_cvtss_f32(min_v);
})

/**
 * @brief Finds the minimum in an array of f32's using SIMD instructions with 256 bits registers
 */
BENCHMARK_FN(f32_min_simd256, {
	volatile float min = F32[0];
	__m256 min_v = _mm256_set1_ps(min);
	for (int i = 1; i < N; i += 8) {
		__m256 v = _mm256_load_ps(&F32[i]);
		min_v = _mm256_min_ps(min_v, v);
	}
	min = _mm256_cvtss_f32(min_v);
})

/**
 * @brief Finds the minimum in an array of f64's using SIMD instructions with 128 bits registers
 */
BENCHMARK_FN(f64_min_simd128, {
	volatile double min = F64[0];
	__m128d min_v = _mm_set1_pd(min);
	for (int i = 1; i < N; i += 2) {
		__m128d v = _mm_load_pd(&F64[i]);
		min_v = _mm_min_pd(min_v, v);
	}
	min = _mm_cvtsd_f64(min_v);
})

/**
 * @brief Finds the minimum in an array of f64's using SIMD instructions with 256 bits registers
 */
BENCHMARK_FN(f64_min_simd256, {
	volatile double min = F64[0];
	__m256d min_v = _mm256_set1_pd(min);
	for (int i = 1; i < N; i += 4) {
		__m256d v = _mm256_load_pd(&F64[i]);
		min_v = _mm256_min_pd(min_v, v);
	}
	min = _mm256_cvtsd_f64(min_v);
})

/**
 * @brief Run the benchmarks for array min
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(1000, oss)
					  .RUN(u8_min_no_simd)
					  .RUN(u8_min_simd128)
					  .RUN(u8_min_simd256)
					  .RUN(u16_min_no_simd)
					  .RUN(u16_min_simd128)
					  .RUN(u16_min_simd256)
					  .RUN(u32_min_no_simd)
					  .RUN(u32_min_simd128)
					  .RUN(u32_min_simd256)
					  .RUN(u64_min_no_simd)
					  .RUN(f32_min_no_simd)
					  .RUN(f32_min_simd128)
					  .RUN(f32_min_simd256)
					  .RUN(f64_min_no_simd)
					  .RUN(f64_min_simd128)
					  .RUN(f64_min_simd256)
					  .doNotOptimizeAway(U8)
					  .doNotOptimizeAway(U16)
					  .doNotOptimizeAway(U32)
					  .doNotOptimizeAway(U64)
					  .doNotOptimizeAway(F32)
					  .doNotOptimizeAway(F64)
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
