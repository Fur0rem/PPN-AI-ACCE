/**
 * @file codelets/codelet_polybench_precond_c_setuppc_line63_2.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) precond_c_setuppc_line63_2 codelet
 */

#include "bench_common.hpp"

int k = 0;
int j = 0;
int i = 0;
int q = 0;
double dpcelm[2400][5][5][5];
int ie = 0;
double g1m1_s[6][5][5][5];
int isize = 2;
double dxtm1_2[5][5];
double rdtime = 1600.000000;
double bm1_s[6][5][5][5];

BENCHMARK_FN(precond_c_setuppc_line63_loop_c_2_c, {
	int __k_0__ = k;
	int __j_1__ = j;
	int __i_2__ = i;
	int __q_3__ = q;

	{
		for (__k_0__ = 0; __k_0__ <= 4; __k_0__ += 1) {
			for (__j_1__ = 0; __j_1__ <= 4; __j_1__ += 1) {
				for (__i_2__ = 0; __i_2__ <= 4; __i_2__ += 1) {
					/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
					/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
					int _lu_fringe_2 = 4;
					for (__q_3__ = 0; __q_3__ <= 4 - _lu_fringe_2; __q_3__ += 4) {
						dpcelm[ie][__k_0__][__j_1__][__i_2__] = dpcelm[ie][__k_0__][__j_1__][__i_2__] +
																g1m1_s[isize][__k_0__][__j_1__][__q_3__] * dxtm1_2[__q_3__][__i_2__] +
																g1m1_s[isize][__k_0__][__q_3__][__i_2__] * dxtm1_2[__q_3__][__j_1__] +
																g1m1_s[isize][__q_3__][__j_1__][__i_2__] * dxtm1_2[__q_3__][__k_0__];
						{
							dpcelm[ie][__k_0__][__j_1__][__i_2__] =
								dpcelm[ie][__k_0__][__j_1__][__i_2__] +
								g1m1_s[isize][__k_0__][__j_1__][__q_3__ + 1] * dxtm1_2[__q_3__ + 1][__i_2__] +
								g1m1_s[isize][__k_0__][__q_3__ + 1][__i_2__] * dxtm1_2[__q_3__ + 1][__j_1__] +
								g1m1_s[isize][__q_3__ + 1][__j_1__][__i_2__] * dxtm1_2[__q_3__ + 1][__k_0__];
						}
						{
							dpcelm[ie][__k_0__][__j_1__][__i_2__] =
								dpcelm[ie][__k_0__][__j_1__][__i_2__] +
								g1m1_s[isize][__k_0__][__j_1__][__q_3__ + 2] * dxtm1_2[__q_3__ + 2][__i_2__] +
								g1m1_s[isize][__k_0__][__q_3__ + 2][__i_2__] * dxtm1_2[__q_3__ + 2][__j_1__] +
								g1m1_s[isize][__q_3__ + 2][__j_1__][__i_2__] * dxtm1_2[__q_3__ + 2][__k_0__];
						}
						{
							dpcelm[ie][__k_0__][__j_1__][__i_2__] =
								dpcelm[ie][__k_0__][__j_1__][__i_2__] +
								g1m1_s[isize][__k_0__][__j_1__][__q_3__ + 3] * dxtm1_2[__q_3__ + 3][__i_2__] +
								g1m1_s[isize][__k_0__][__q_3__ + 3][__i_2__] * dxtm1_2[__q_3__ + 3][__j_1__] +
								g1m1_s[isize][__q_3__ + 3][__j_1__][__i_2__] * dxtm1_2[__q_3__ + 3][__k_0__];
						}
					}
					for (; __q_3__ <= 4; __q_3__ += 1) {
						dpcelm[ie][__k_0__][__j_1__][__i_2__] = dpcelm[ie][__k_0__][__j_1__][__i_2__] +
																g1m1_s[isize][__k_0__][__j_1__][__q_3__] * dxtm1_2[__q_3__][__i_2__] +
																g1m1_s[isize][__k_0__][__q_3__][__i_2__] * dxtm1_2[__q_3__][__j_1__] +
																g1m1_s[isize][__q_3__][__j_1__][__i_2__] * dxtm1_2[__q_3__][__k_0__];
					}
					dpcelm[ie][__k_0__][__j_1__][__i_2__] =
						0.005 * dpcelm[ie][__k_0__][__j_1__][__i_2__] + rdtime * bm1_s[isize][__k_0__][__j_1__][__i_2__];
				}
			}
		}
	}

	k = __k_0__;
	j = __j_1__;
	i = __i_2__;
	q = __q_3__;
})

/**
 * @brief Run the benchmarks for precond_c_setuppc_line63_loop_c_2_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(precond_c_setuppc_line63_loop_c_2_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}