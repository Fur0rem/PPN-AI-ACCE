/**
 * @file benchmarks/codelet_polybench_adapt_c_remap_line868_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) adapt_c_remap_line868_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
int kk = 0;
int jj = 0;
int ii = 0;
double yone[2][5][5][5];
double ixmc1[5][5];
double x[5][5][5];
double ixmc2[5][5];
double ytwo[4][5][5][5];
double ixtmc1[5][5];
double ixtmc2[5][5];

BENCHMARK_FN(adapt_c_remap_line868_loop_c_0_c, {
	int __i_0__ = i;
	int __kk_1__ = kk;
	int __jj_2__ = jj;
	int __ii_3__ = ii;

	{
		NO_VECTORISE_NO_UNROLL
		for (__i_0__ = 0; __i_0__ <= 4; __i_0__ += 1) {
			NO_VECTORISE_NO_UNROLL
			for (__kk_1__ = 0; __kk_1__ <= 4; __kk_1__ += 1) {
				NO_VECTORISE_NO_UNROLL
				for (__jj_2__ = 0; __jj_2__ <= 4; __jj_2__ += 1) {
					NO_VECTORISE_NO_UNROLL
					for (__ii_3__ = 0; __ii_3__ <= 4; __ii_3__ += 1) {
						yone[0][__i_0__][__jj_2__][__ii_3__] =
							yone[0][__i_0__][__jj_2__][__ii_3__] + ixmc1[__kk_1__][__ii_3__] * x[__i_0__][__jj_2__][__kk_1__];
						yone[1][__i_0__][__jj_2__][__ii_3__] =
							yone[1][__i_0__][__jj_2__][__ii_3__] + ixmc2[__kk_1__][__ii_3__] * x[__i_0__][__jj_2__][__kk_1__];
					}
				}
			}
			NO_VECTORISE_NO_UNROLL
			for (__kk_1__ = 0; __kk_1__ <= 4; __kk_1__ += 1) {
				NO_VECTORISE_NO_UNROLL
				for (__jj_2__ = 0; __jj_2__ <= 4; __jj_2__ += 1) {
					NO_VECTORISE_NO_UNROLL
					for (__ii_3__ = 0; __ii_3__ <= 4; __ii_3__ += 1) {
						ytwo[0][__jj_2__][__i_0__][__ii_3__] =
							ytwo[0][__jj_2__][__i_0__][__ii_3__] + yone[0][__i_0__][__kk_1__][__ii_3__] * ixtmc1[__jj_2__][__kk_1__];
						ytwo[1][__jj_2__][__i_0__][__ii_3__] =
							ytwo[1][__jj_2__][__i_0__][__ii_3__] + yone[0][__i_0__][__kk_1__][__ii_3__] * ixtmc2[__jj_2__][__kk_1__];
						ytwo[2][__jj_2__][__i_0__][__ii_3__] =
							ytwo[2][__jj_2__][__i_0__][__ii_3__] + yone[1][__i_0__][__kk_1__][__ii_3__] * ixtmc1[__jj_2__][__kk_1__];
						ytwo[3][__jj_2__][__i_0__][__ii_3__] =
							ytwo[3][__jj_2__][__i_0__][__ii_3__] + yone[1][__i_0__][__kk_1__][__ii_3__] * ixtmc2[__jj_2__][__kk_1__];
					}
				}
			}
		}
	}

	i = __i_0__;
	kk = __kk_1__;
	jj = __jj_2__;
	ii = __ii_3__;
})

/**
 * @brief Run the benchmarks for adapt_c_remap_line868_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(adapt_c_remap_line868_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}