/**
 * @file benchmarks/common.hpp
 * @brief Common functions and macros for the benchmarks
 */

#define ANKERL_NANOBENCH_IMPLEMENT ///< Needed to use nanobench
#include "../nanobench/src/include/nanobench.h"
#include <immintrin.h>

#define STRINGIFY_INTERN(s) #s					///< Internal helper macro to stringify a value (we need two macros to stringify a value)
#define STRINGIFY(s)		STRINGIFY_INTERN(s) ///< Helper macro to stringify a value

/**
 * @brief Add a label before and after the code to be able to find it in the assembly
 */
#define LABEL(name, code)                                                                                                                  \
	/*asm volatile(STRINGIFY(___##name##_start_PPN_LABEL:));*/                                                                             \
	code;                                                                                                                                  \
	asm volatile(STRINGIFY(___##name##_end_PPN_LABEL:));

/**
 * @brief Macro to define a benchmark function, placing labels before and after the code
 * @param[in] name Name of the benchmark function
 */
#define BENCHMARK_FN(name, code)                                                                                                           \
	void ___##name##_start_PPN_LABEL() {                                                                                                   \
		code;                                                                                                                              \
		asm volatile(STRINGIFY(___##name##_end_PPN_LABEL:));                                                                               \
	}

/**
 * @brief Macro to run a benchmark function inside the main function
 * @param[in] name Name of the benchmark function
 */
#define RUN(name) run(STRINGIFY(___##name##_start_PPN_LABEL), ___##name##_start_PPN_LABEL).doNotOptimizeAway(___##name##_start_PPN_LABEL)

/**
 * @brief Macro to initialise the benchmark with a given number of iterations and output buffer
 * @param[in] nb_iters Number of iterations
 * @param[in] out_buf Output buffer
 */
#define BENCHMARK_INIT(nb_iters, out_buf)                                                                                                  \
	ankerl::nanobench::Bench().minEpochIterations((nb_iters)).performanceCounters(true).output(&(out_buf))

/**
 * @brief Concatenate two vectors of nanobench results
 * @param[in] results Vector to concatenate to
 * @param[in] other_results Vector to concatenate
 */
inline void concat_results(std::vector<ankerl::nanobench::Result>& results, std::vector<ankerl::nanobench::Result> const& other_results) {
	results.reserve(results.size() + other_results.size());
	for (auto const& res : other_results) {
		results.push_back(res);
	}
}

/**
 * @brief Disable vectorisation for a loop
 */
#define NO_VECTORISE _Pragma("GCC ivdep")

/**
 * @brief Disable vectorisation and unrolling for a loop
 */
#define NO_VECTORISE_NO_UNROLL _Pragma("GCC ivdep") _Pragma("GCC unroll 1")

/**
 * @brief Optimise the code for size
 */
#define OPTIMISE_SIZE _Pragma("GCC optimize(\"Os\")")

/**
 * @brief Optimise the code for speed
 */
#define OPTIMISE_SPEED _Pragma("GCC optimize(\"O3\")")