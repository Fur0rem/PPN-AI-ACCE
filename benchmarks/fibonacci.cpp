/**
 * @file benchmarks/fibonacci.cpp
 * @brief Benchmark for Fibonacci
 */

#include "bench_common.hpp"
#include <functional>

constexpr int N = 35;			 ///< The Fibonacci number to compute
volatile int fib_result;		 ///< The result of the Fibonacci computation
volatile float fib_float_result; ///< The result of the Fibonacci computation

/**
 * @brief Compute the Nth Fibonacci number using an iterative algorithm
 */
BENCHMARK_FN(fib_iter, {
	int a = 0;
	int b = 1;
	int c;
	for (int i = 2; i <= N; i++) {
		c = a + b;
		a = b;
		b = c;
	}
	fib_result = b;
})

/**
 * @brief Compute the Nth Fibonacci number using floating point numbers and a slow exponential algorithm
 */
BENCHMARK_FN(fib_float_slow_exp, {
	float phi = (1 + sqrt(5)) / 2;
	float psi = (1 - sqrt(5)) / 2;
	float phi_N = 1.0;
	float psi_N = 1.0;
	for (int i = 0; i < N; i++) {
		phi_N *= phi;
		psi_N *= psi;
	}
	float fib = (phi_N - psi_N) / sqrt(5);
	fib_float_result = fib;
})

/**
 * @brief Compute the Nth Fibonacci number using floating point numbers and a fast exponential algorithm
 */
BENCHMARK_FN(fib_float_fast_exp, {
	float phi = (1 + sqrt(5)) / 2;
	float psi = (1 - sqrt(5)) / 2;
	float phi_N = pow(phi, N);
	float psi_N = pow(psi, N);
	float fib = (phi_N - psi_N) / sqrt(5);
	fib_float_result = fib;
})

/**
 * @brief Compute the Nth Fibonacci number using a recursive algorithm
 */
BENCHMARK_FN(fib_recursive, {
	int result = 0;
	asm volatile("mov %[N], %%eax\n"
				 "cmp $1, %%eax\n"
				 "jle fib_recursive_asm_end\n"
				 "mov $0, %%ebx\n"
				 "mov $1, %%ecx\n"
				 "mov $2, %%edx\n"
				 "fib_recursive_asm_loop:\n"
				 "cmp %%eax, %%edx\n"
				 "jge fib_recursive_asm_end_loop\n"
				 "add %%ecx, %%ebx\n"
				 "mov %%ebx, %%ecx\n"
				 "mov %%edx, %%ebx\n"
				 "inc %%edx\n"
				 "jmp fib_recursive_asm_loop\n"
				 "fib_recursive_asm_end_loop:\n"
				 "mov %%ecx, %[result]\n"
				 "fib_recursive_asm_end:\n"
				 : [result] "=r"(result)
				 : [N] "r"(N)
				 : "eax", "ebx", "ecx", "edx");
	fib_result = result;
})

/**
 * @brief Run the benchmarks for fibonacci
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(fib_iter).RUN(fib_float_fast_exp).RUN(fib_float_slow_exp).RUN(fib_recursive).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
