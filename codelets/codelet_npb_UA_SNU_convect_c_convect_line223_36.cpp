/**
 * @file codelets/codelet_polybench_convect_c_convect_line223_36.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) convect_c_convect_line223_36 codelet
 */

#include "bench_common.hpp"

int k = 0;
int j = 0;
int i = 0;
double trhs[2400][5][5][5];
int iel = 0;
double bm1_s[6][5][5][5];
int isize = 2;
double ta1[2400][5][5][5];
double rdtime = 1600.000000;
double tempa[5][5][5];
double dtime = 0.000625;

BENCHMARK_FN(convect_c_convect_line223_loop_c_36_c, {
	int __k_0__ = k;
	int __j_1__ = j;
	int __i_2__ = i;

	{
		/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
		/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
		int _lu_fringe_70 = 4;
		NO_VECTORISE_NO_UNROLL
		for (__j_1__ = 0; __j_1__ <= 4 - _lu_fringe_70; __j_1__ += 4) {
			NO_VECTORISE_NO_UNROLL
			for (__k_0__ = 0; __k_0__ <= 4; __k_0__ += 1) {
				NO_VECTORISE
				for (__i_2__ = 0; __i_2__ <= 4; __i_2__ += 1) {
					trhs[iel][__k_0__][__j_1__][__i_2__] =
						bm1_s[isize][__k_0__][__j_1__][__i_2__] *
						(ta1[iel][__k_0__][__j_1__][__i_2__] * rdtime + tempa[__k_0__][__j_1__][__i_2__]);
					ta1[iel][__k_0__][__j_1__][__i_2__] = ta1[iel][__k_0__][__j_1__][__i_2__] + tempa[__k_0__][__j_1__][__i_2__] * dtime;
				}
				NO_VECTORISE
				for (__i_2__ = 0; __i_2__ <= 4; __i_2__ += 1) {
					trhs[iel][__k_0__][__j_1__ + 1][__i_2__] =
						bm1_s[isize][__k_0__][__j_1__ + 1][__i_2__] *
						(ta1[iel][__k_0__][__j_1__ + 1][__i_2__] * rdtime + tempa[__k_0__][__j_1__ + 1][__i_2__]);
					ta1[iel][__k_0__][__j_1__ + 1][__i_2__] =
						ta1[iel][__k_0__][__j_1__ + 1][__i_2__] + tempa[__k_0__][__j_1__ + 1][__i_2__] * dtime;
				}
				NO_VECTORISE
				for (__i_2__ = 0; __i_2__ <= 4; __i_2__ += 1) {
					trhs[iel][__k_0__][__j_1__ + 2][__i_2__] =
						bm1_s[isize][__k_0__][__j_1__ + 2][__i_2__] *
						(ta1[iel][__k_0__][__j_1__ + 2][__i_2__] * rdtime + tempa[__k_0__][__j_1__ + 2][__i_2__]);
					ta1[iel][__k_0__][__j_1__ + 2][__i_2__] =
						ta1[iel][__k_0__][__j_1__ + 2][__i_2__] + tempa[__k_0__][__j_1__ + 2][__i_2__] * dtime;
				}
				NO_VECTORISE
				for (__i_2__ = 0; __i_2__ <= 4; __i_2__ += 1) {
					trhs[iel][__k_0__][__j_1__ + 3][__i_2__] =
						bm1_s[isize][__k_0__][__j_1__ + 3][__i_2__] *
						(ta1[iel][__k_0__][__j_1__ + 3][__i_2__] * rdtime + tempa[__k_0__][__j_1__ + 3][__i_2__]);
					ta1[iel][__k_0__][__j_1__ + 3][__i_2__] =
						ta1[iel][__k_0__][__j_1__ + 3][__i_2__] + tempa[__k_0__][__j_1__ + 3][__i_2__] * dtime;
				}
			}
		}
		NO_VECTORISE_NO_UNROLL
		for (; __j_1__ <= 4; __j_1__ += 1) {
			NO_VECTORISE_NO_UNROLL
			for (__k_0__ = 0; __k_0__ <= 4; __k_0__ += 1) {
				NO_VECTORISE_NO_UNROLL
				for (__i_2__ = 0; __i_2__ <= 4; __i_2__ += 1) {
					trhs[iel][__k_0__][__j_1__][__i_2__] =
						bm1_s[isize][__k_0__][__j_1__][__i_2__] *
						(ta1[iel][__k_0__][__j_1__][__i_2__] * rdtime + tempa[__k_0__][__j_1__][__i_2__]);
					ta1[iel][__k_0__][__j_1__][__i_2__] = ta1[iel][__k_0__][__j_1__][__i_2__] + tempa[__k_0__][__j_1__][__i_2__] * dtime;
				}
			}
		}
	}

	k = __k_0__;
	j = __j_1__;
	i = __i_2__;
})

/**
 * @brief Run the benchmarks for convect_c_convect_line223_loop_c_36_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(convect_c_convect_line223_loop_c_36_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}