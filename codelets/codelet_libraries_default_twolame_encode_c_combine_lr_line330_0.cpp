/**
 * @file codelets/codelet_libraries_default_encode_c_combine_lr_line330_0.cpp
 * @brief Benchmark extracted from the Libraries default codelet
 */

#include "bench_common.hpp"

int sb = 0;
int sblimit = 30;
int sample = 0;
int gr = 0;
double joint_sample[3][12][32];
double sb_sample[2][3][12][32];

BENCHMARK_FN(encode_c_combine_lr_line330_loop_c_0_c, {
	int __sb_0__ = sb;
	int __sample_1__ = sample;
	int __gr_2__ = gr;

	{
		NO_VECTORISE_NO_UNROLL
		for (__sb_0__ = 0; __sb_0__ <= sblimit - 1; __sb_0__ += 1) {
			NO_VECTORISE_NO_UNROLL
			for (__sample_1__ = 0; __sample_1__ <= 11; __sample_1__ += 1) {
				NO_VECTORISE_NO_UNROLL
				for (__gr_2__ = 0; __gr_2__ <= 2; __gr_2__ += 1) {
					joint_sample[__gr_2__][__sample_1__][__sb_0__] =
						0.5 * (sb_sample[0][__gr_2__][__sample_1__][__sb_0__] + sb_sample[1][__gr_2__][__sample_1__][__sb_0__]);
				}
			}
		}
	}

	sb = __sb_0__;
	sample = __sample_1__;
	gr = __gr_2__;
})

/**
 * @brief Run the benchmarks for encode_c_combine_lr_line330_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(encode_c_combine_lr_line330_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
