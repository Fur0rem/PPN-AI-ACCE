/**
 * @file codelets/codelet_polybench_adapt_c_remap_line894_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) adapt_c_remap_line894_0 codelet
 */

#include "bench_common.hpp"

int iz = 0;
int kk = 0;
int jj = 0;
int ii = 0;
double y[5][5][5];
double ytwo[4][5][5][5];
double ixtmc1[5][5];
double y_1[7][5][5][5];
double ixtmc2[5][5];

BENCHMARK_FN(adapt_c_remap_line894_loop_c_0_c, {
	int __iz_0__ = iz;
	int __kk_1__ = kk;
	int __jj_2__ = jj;
	int __ii_3__ = ii;

	{
		NO_VECTORISE_NO_UNROLL
		for (__iz_0__ = 0; __iz_0__ <= 4; __iz_0__ += 1) {
			NO_VECTORISE_NO_UNROLL
			for (__kk_1__ = 0; __kk_1__ <= 4; __kk_1__ += 1) {
				NO_VECTORISE_NO_UNROLL
				for (__jj_2__ = 0; __jj_2__ <= 4; __jj_2__ += 1) {
					NO_VECTORISE_NO_UNROLL
					for (__ii_3__ = 0; __ii_3__ <= 4; __ii_3__ += 1) {
						y[__jj_2__][__iz_0__][__ii_3__] =
							y[__jj_2__][__iz_0__][__ii_3__] + ytwo[0][__iz_0__][__kk_1__][__ii_3__] * ixtmc1[__jj_2__][__kk_1__];
						y_1[0][__jj_2__][__iz_0__][__ii_3__] =
							y_1[0][__jj_2__][__iz_0__][__ii_3__] + ytwo[2][__iz_0__][__kk_1__][__ii_3__] * ixtmc1[__jj_2__][__kk_1__];
						y_1[1][__jj_2__][__iz_0__][__ii_3__] =
							y_1[1][__jj_2__][__iz_0__][__ii_3__] + ytwo[1][__iz_0__][__kk_1__][__ii_3__] * ixtmc1[__jj_2__][__kk_1__];
						y_1[2][__jj_2__][__iz_0__][__ii_3__] =
							y_1[2][__jj_2__][__iz_0__][__ii_3__] + ytwo[3][__iz_0__][__kk_1__][__ii_3__] * ixtmc1[__jj_2__][__kk_1__];
						y_1[3][__jj_2__][__iz_0__][__ii_3__] =
							y_1[3][__jj_2__][__iz_0__][__ii_3__] + ytwo[0][__iz_0__][__kk_1__][__ii_3__] * ixtmc2[__jj_2__][__kk_1__];
						y_1[4][__jj_2__][__iz_0__][__ii_3__] =
							y_1[4][__jj_2__][__iz_0__][__ii_3__] + ytwo[2][__iz_0__][__kk_1__][__ii_3__] * ixtmc2[__jj_2__][__kk_1__];
						y_1[5][__jj_2__][__iz_0__][__ii_3__] =
							y_1[5][__jj_2__][__iz_0__][__ii_3__] + ytwo[1][__iz_0__][__kk_1__][__ii_3__] * ixtmc2[__jj_2__][__kk_1__];
						y_1[6][__jj_2__][__iz_0__][__ii_3__] =
							y_1[6][__jj_2__][__iz_0__][__ii_3__] + ytwo[3][__iz_0__][__kk_1__][__ii_3__] * ixtmc2[__jj_2__][__kk_1__];
					}
				}
			}
		}
	}

	iz = __iz_0__;
	kk = __kk_1__;
	jj = __jj_2__;
	ii = __ii_3__;
})

/**
 * @brief Run the benchmarks for adapt_c_remap_line894_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(adapt_c_remap_line894_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}