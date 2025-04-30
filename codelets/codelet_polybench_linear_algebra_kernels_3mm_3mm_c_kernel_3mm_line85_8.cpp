/**
 * @file codelets/codelet_polybench_3mm_c_kernel_3mm_line85_8.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 0;
int ni = 800;
int j = 0;
int nj = 900;
double e[800 + 0][900 + 0];
int k = 0;
int nk = 1000;
double a[800 + 0][1000 + 0];
double b[1000 + 0][900 + 0];

OPTIMISE_SIZE
BENCHMARK_FN(3mm_c_kernel_3mm_line85_loop_c_8_c, {
	int __i_0__ = i;
	int __j_1__ = j;
	int __k_2__ = k;

	{
		/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
		/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
		int _lu_fringe_14 = (((ni - 1 + 1 - 0) % 1 == 0 ? (ni - 1 + 1 - 0) / 1 : (ni - 1 + 1 - 0) / 1 + 1)) % 4 == 0 ? 0 : 4;
		for (__i_0__ = 0; __i_0__ <= ni - 1 - _lu_fringe_14; __i_0__ += 4) {
			for (__j_1__ = 0; __j_1__ <= nj - 1; __j_1__ += 1) {
				e[__i_0__][__j_1__] = 0.0;
				for (__k_2__ = 0; __k_2__ <= nk - 1; __k_2__ += 1) {
					e[__i_0__][__j_1__] += a[__i_0__][__k_2__] * b[__k_2__][__j_1__];
				}
				e[__i_0__ + 1][__j_1__] = 0.0;
				for (__k_2__ = 0; __k_2__ <= nk - 1; __k_2__ += 1) {
					e[__i_0__ + 1][__j_1__] += a[__i_0__ + 1][__k_2__] * b[__k_2__][__j_1__];
				}
				e[__i_0__ + 2][__j_1__] = 0.0;
				for (__k_2__ = 0; __k_2__ <= nk - 1; __k_2__ += 1) {
					e[__i_0__ + 2][__j_1__] += a[__i_0__ + 2][__k_2__] * b[__k_2__][__j_1__];
				}
				e[__i_0__ + 3][__j_1__] = 0.0;
				for (__k_2__ = 0; __k_2__ <= nk - 1; __k_2__ += 1) {
					e[__i_0__ + 3][__j_1__] += a[__i_0__ + 3][__k_2__] * b[__k_2__][__j_1__];
				}
			}
		}
		for (; __i_0__ <= ni - 1; __i_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= nj - 1; __j_1__ += 1) {
				e[__i_0__][__j_1__] = 0.0;
				for (__k_2__ = 0; __k_2__ <= nk - 1; __k_2__ += 1) {
					e[__i_0__][__j_1__] += a[__i_0__][__k_2__] * b[__k_2__][__j_1__];
				}
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
	k = __k_2__;
})

/**
 * @brief Run the benchmarks for 3mm_c_kernel_3mm_line85_loop_c_8_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(3, oss).RUN(3mm_c_kernel_3mm_line85_loop_c_8_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}