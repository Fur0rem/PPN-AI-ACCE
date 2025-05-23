/**
 * @file codelets/codelet_polybench_gemver_c_kernel_gemver_line105_44.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 2000;
int n = 2000;
int j = 2000;
double x[2000 + 0];
double beta = 1.200000;
double a[2000 + 0][2000 + 0];
double y[2000 + 0];

BENCHMARK_FN(gemver_c_kernel_gemver_line105_loop_c_44_c, {
	int __i_0__ = i;
	int __j_1__ = j;

	{
		/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
		/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
		int _lu_fringe_46 = (((n - 1 + 1 - 0) % 1 == 0 ? (n - 1 + 1 - 0) / 1 : (n - 1 + 1 - 0) / 1 + 1)) % 4 == 0 ? 0 : 4;
		for (__j_1__ = 0; __j_1__ <= n - 1 - _lu_fringe_46; __j_1__ += 4) {
			for (__i_0__ = 0; __i_0__ <= n - 1; __i_0__ += 1) {
				x[__i_0__] = x[__i_0__] + beta * a[__j_1__][__i_0__] * y[__j_1__];
				x[__i_0__] = x[__i_0__] + beta * a[__j_1__ + 1][__i_0__] * y[__j_1__ + 1];
				x[__i_0__] = x[__i_0__] + beta * a[__j_1__ + 2][__i_0__] * y[__j_1__ + 2];
				x[__i_0__] = x[__i_0__] + beta * a[__j_1__ + 3][__i_0__] * y[__j_1__ + 3];
			}
		}
		for (; __j_1__ <= n - 1; __j_1__ += 1) {
			for (__i_0__ = 0; __i_0__ <= n - 1; __i_0__ += 1) {
				x[__i_0__] = x[__i_0__] + beta * a[__j_1__][__i_0__] * y[__j_1__];
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
})

/**
 * @brief Run the benchmarks for gemver_c_kernel_gemver_line105_loop_c_44_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(3, oss).RUN(gemver_c_kernel_gemver_line105_loop_c_44_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}