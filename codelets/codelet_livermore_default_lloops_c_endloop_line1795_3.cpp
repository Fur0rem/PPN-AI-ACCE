/**
 * @file codelets/codelet_livermore_default_lloops_c_endloop_line1795_3.cpp
 * @brief Benchmark extracted from the Livermore loops codelet
 */

#include "bench_common.hpp"

long k = 0;
long inner_loops = 101;
long i = 0;
long m = 0;
double sumcheck[3][25];
long section = 0;
double px[101][25];

BENCHMARK_FN(lloops_c_endloop_line1795_loop_c_3_c, {
	int __k_0__ = k;
	int __i_1__ = i;

	{
		for (__k_0__ = 0; ((long)__k_0__) < inner_loops; __k_0__++) {
			/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
			/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
			int _lu_fringe_3 = 8;
			for (__i_1__ = 0; __i_1__ <= 24 - _lu_fringe_3; __i_1__ += 8) {
				m = ((long)(25 * __k_0__ + __i_1__ + 1));
				sumcheck[section][21] = sumcheck[section][21] + px[__k_0__][__i_1__] * ((double)m);
				{
					m = ((long)(25 * __k_0__ + (__i_1__ + 1) + 1));
					sumcheck[section][21] = sumcheck[section][21] + px[__k_0__][__i_1__ + 1] * ((double)m);
				}
				{
					m = ((long)(25 * __k_0__ + (__i_1__ + 2) + 1));
					sumcheck[section][21] = sumcheck[section][21] + px[__k_0__][__i_1__ + 2] * ((double)m);
				}
				{
					m = ((long)(25 * __k_0__ + (__i_1__ + 3) + 1));
					sumcheck[section][21] = sumcheck[section][21] + px[__k_0__][__i_1__ + 3] * ((double)m);
				}
				{
					m = ((long)(25 * __k_0__ + (__i_1__ + 4) + 1));
					sumcheck[section][21] = sumcheck[section][21] + px[__k_0__][__i_1__ + 4] * ((double)m);
				}
				{
					m = ((long)(25 * __k_0__ + (__i_1__ + 5) + 1));
					sumcheck[section][21] = sumcheck[section][21] + px[__k_0__][__i_1__ + 5] * ((double)m);
				}
				{
					m = ((long)(25 * __k_0__ + (__i_1__ + 6) + 1));
					sumcheck[section][21] = sumcheck[section][21] + px[__k_0__][__i_1__ + 6] * ((double)m);
				}
				{
					m = ((long)(25 * __k_0__ + (__i_1__ + 7) + 1));
					sumcheck[section][21] = sumcheck[section][21] + px[__k_0__][__i_1__ + 7] * ((double)m);
				}
			}
			for (; __i_1__ <= 24; __i_1__ += 1) {
				m = ((long)(25 * __k_0__ + __i_1__ + 1));
				sumcheck[section][21] = sumcheck[section][21] + px[__k_0__][__i_1__] * ((double)m);
			}
		}
	}

	k = __k_0__;
	i = __i_1__;
})
/**
 * @brief Run the benchmarks for lloops_c_endloop_line1795_main_c
 */
int main() {
	std::ostringstream oss;
	auto result =
		BENCHMARK_INIT(10, oss).RUN(lloops_c_endloop_line1795_loop_c_3_c).doNotOptimizeAway(sumcheck).doNotOptimizeAway(px).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
