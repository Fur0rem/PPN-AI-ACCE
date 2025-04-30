/**
 * @file codelets/codelet_polybench_2mm_c_kernel_2mm_line96_0.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 800;
int ni = 800;
int j = 900;
int nl = 1200;
double d[800 + 0][1200 + 0];
double beta = 1.200000;
int k = 1100;
int nj = 900;
double tmp[800 + 0][900 + 0];
double c[900 + 0][1200 + 0];

BENCHMARK_FN(2mm_c_kernel_2mm_line96_loop_c_0_c, {
	int __i_0__ = i;
	int __j_1__ = j;
	int __k_2__ = k;

	{
		for (__i_0__ = 0; __i_0__ <= ni - 1; __i_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= nl - 1; __j_1__ += 1) {
				d[__i_0__][__j_1__] *= beta;
				for (__k_2__ = 0; __k_2__ <= nj - 1; __k_2__ += 1) {
					d[__i_0__][__j_1__] += tmp[__i_0__][__k_2__] * c[__k_2__][__j_1__];
				}
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
	k = __k_2__;
})

/**
 * @brief Run the benchmarks for 2mm_c_kernel_2mm_line96_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(3, oss).RUN(2mm_c_kernel_2mm_line96_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}