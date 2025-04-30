/**
 * @file codelets/mat_mul.cpp
 * @brief Benchmark for matrix multiplication
 */

#include "bench_common.hpp"

#define DECLARE_MATRIX(size)                                                                                                               \
	constexpr double m##size##_a[size][size] = {0};                                                                                        \
	constexpr double m##size##_b[size][size] = {0};                                                                                        \
	double m##size##_c[size][size] = {0};

DECLARE_MATRIX(4)
DECLARE_MATRIX(8)
DECLARE_MATRIX(16)
DECLARE_MATRIX(32)
DECLARE_MATRIX(64)
DECLARE_MATRIX(128)
DECLARE_MATRIX(256)
DECLARE_MATRIX(512)
DECLARE_MATRIX(1024)
DECLARE_MATRIX(2048)
DECLARE_MATRIX(4096)
DECLARE_MATRIX(8192)

/**
 * @brief Instantiate the matrix multiplication function for a given size with ijk loop order
 * @param[in] size Size of the matrix
 */
#define MAT_MULT_IJK(size)                                                                                                                 \
	BENCHMARK_FN(mat_mult_##size##_ijk, {                                                                                                  \
		for (int i = 0; i < (size); i++) {                                                                                                 \
			for (int j = 0; j < (size); j++) {                                                                                             \
				for (int k = 0; k < (size); k++) {                                                                                         \
					m##size##_c[i][j] += m##size##_a[i][k] * m##size##_b[k][j];                                                            \
				}                                                                                                                          \
			}                                                                                                                              \
		}                                                                                                                                  \
	})

/**
 * @brief Instantiate the matrix multiplication function for a given size with jki loop order
 * @param[in] size Size of the matrix
 */
#define MAT_MULT_JKI(size)                                                                                                                 \
	BENCHMARK_FN(mat_mult_##size##_jki, {                                                                                                  \
		for (int j = 0; j < (size); j++) {                                                                                                 \
			for (int k = 0; k < (size); k++) {                                                                                             \
				for (int i = 0; i < (size); i++) {                                                                                         \
					m##size##_c[i][j] += m##size##_a[i][k] * m##size##_b[k][j];                                                            \
				}                                                                                                                          \
			}                                                                                                                              \
		}                                                                                                                                  \
	})

/**
 * @brief Instantiate the matrix multiplication function for a given size with kij loop order
 * @param[in] size Size of the matrix
 */
#define MAT_MULT_KIJ(size)                                                                                                                 \
	BENCHMARK_FN(mat_mult_##size##_kij, {                                                                                                  \
		NO_VECTORISE_NO_UNROLL                                                                                                             \
		for (int k = 0; k < (size); k++) {                                                                                                 \
			NO_VECTORISE_NO_UNROLL                                                                                                         \
			for (int i = 0; i < (size); i++) {                                                                                             \
				NO_VECTORISE_NO_UNROLL                                                                                                     \
				for (int j = 0; j < (size); j++) {                                                                                         \
					m##size##_c[i][j] += m##size##_a[i][k] * m##size##_b[k][j];                                                            \
				}                                                                                                                          \
			}                                                                                                                              \
		}                                                                                                                                  \
	})

/**
 * @brief Instantiate the matrix multiplication function for a given size with ikj loop order
 * @param[in] size Size of the matrix
 */
#define MAT_MULT_IKJ(size)                                                                                                                 \
	OPTIMISE_SIZE                                                                                                                          \
	BENCHMARK_FN(mat_mult_##size##_ikj, {                                                                                                  \
		for (int i = 0; i < (size); i++) {                                                                                                 \
			for (int k = 0; k < (size); k++) {                                                                                             \
				for (int j = 0; j < (size); j++) {                                                                                         \
					m##size##_c[i][j] += m##size##_a[i][k] * m##size##_b[k][j];                                                            \
				}                                                                                                                          \
			}                                                                                                                              \
		}                                                                                                                                  \
	})                                                                                                                                     \
	OPTIMISE_SPEED

/**
 * @brief Instantiate the matrix multiplication function for a given size with jik loop order
 * @param[in] size Size of the matrix
 */
#define MAT_MULT_JIK(size)                                                                                                                 \
	BENCHMARK_FN(mat_mult_##size##_jik, {                                                                                                  \
		for (int j = 0; j < (size); j++) {                                                                                                 \
			for (int i = 0; i < (size); i++) {                                                                                             \
				for (int k = 0; k < (size); k++) {                                                                                         \
					m##size##_c[i][j] += m##size##_a[i][k] * m##size##_b[k][j];                                                            \
				}                                                                                                                          \
			}                                                                                                                              \
		}                                                                                                                                  \
	})

/**
 * @brief Instantiates all loop orders for matrix multiplication for a given size
 * @param[in] size Size of the matrix
 */
#define MAT_MULT(size)                                                                                                                     \
	MAT_MULT_IJK(size)                                                                                                                     \
	MAT_MULT_JKI(size)                                                                                                                     \
	MAT_MULT_KIJ(size)                                                                                                                     \
	MAT_MULT_IKJ(size)                                                                                                                     \
	MAT_MULT_JIK(size)

MAT_MULT(4)
MAT_MULT(8)
MAT_MULT(16)
MAT_MULT(32)
MAT_MULT(64)
MAT_MULT(128)
MAT_MULT(256)
MAT_MULT(512)

#define RUN_ALL(size)                                                                                                                      \
	RUN(mat_mult_##size##_ijk)                                                                                                             \
		.RUN(mat_mult_##size##_jki)                                                                                                        \
		.RUN(mat_mult_##size##_kij)                                                                                                        \
		.RUN(mat_mult_##size##_ikj)                                                                                                        \
		.RUN(mat_mult_##size##_jik)                                                                                                        \
		.doNotOptimizeAway(m##size##_c)                                                                                                    \
		.doNotOptimizeAway(m##size##_a)                                                                                                    \
		.doNotOptimizeAway(m##size##_b)

/**
 * @brief Run the benchmarks for matrix multiplication
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(100, oss).RUN_ALL(4).RUN_ALL(8).RUN_ALL(16).RUN_ALL(32).results();
	concat_results(result, BENCHMARK_INIT(2, oss).output(&oss).RUN_ALL(64).RUN_ALL(128).RUN_ALL(256).RUN_ALL(512).results());

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
