/**
 * @file benchmarks/codelet_polybench_gemver_c_kernel_gemver_line105_id2o1_jl0f4_tl1s128_tl2s128.cpp
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

OPTIMISE_SIZE
BENCHMARK_FN(gemver_c_kernel_gemver_line105_loop_c_44_c, {
	int i0 = i;
	int j_1 = j;

	{
		/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
		/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
		int _lu_fringe_46 = (((n - 1 + 1 - 0) % 1 == 0 ? (n - 1 + 1 - 0) / 1 : (n - 1 + 1 - 0) / 1 + 1)) % 4 == 0 ? 0 : 4;
		int ltvarj1;
		for (ltvarj1 = 0; ltvarj1 <= n - 1 - _lu_fringe_46; ltvarj1 += 512) {
			int ltvari0;
			for (ltvari0 = 0; ltvari0 <= n - 1; ltvari0 += 128) {
				for (j_1 = ltvarj1;
					 j_1 <= (((n - 1 - _lu_fringe_46 < (ltvarj1 + 128 - 1)) ? (n - 1 - _lu_fringe_46) : (ltvarj1 + 128 - 1)));
					 j_1 += 4) {
					for (i0 = ltvari0; i0 <= (((n - 1 < (ltvari0 + 128 - 1)) ? (n - 1) : (ltvari0 + 128 - 1))); i0 += 1) {
						x[i0] = x[i0] + beta * a[j_1][i0] * y[j_1];
						x[i0] = x[i0] + beta * a[j_1 + 1][i0] * y[j_1 + 1];
						x[i0] = x[i0] + beta * a[j_1 + 2][i0] * y[j_1 + 2];
						x[i0] = x[i0] + beta * a[j_1 + 3][i0] * y[j_1 + 3];
					}
				}
			}
		}

		for (; j_1 <= n - 1; j_1 += 1) {
			for (i0 = 0; i0 <= n - 1; i0 += 1) {
				x[i0] = x[i0] + beta * a[j_1][i0] * y[j_1];
			}
		}
	}

	i = i0;
	j = j_1;
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