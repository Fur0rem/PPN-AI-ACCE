/**
 * @file benchmarks/codelet_libraries_default_encode_c_combine_lr_line330_1.cpp
 * @brief Benchmark extracted from the Libraries default codelet
 */

#include "bench_common.hpp"

int sb = 0;
int sblimit = 30;
int sample = 0;
int gr = 0;
double joint_sample[3][12][32];
double sb_sample[2][3][12][32];

BENCHMARK_FN(encode_c_combine_lr_line330_loop_c_1_c, {
	int sb0 = sb;
	int sample1 = sample;
	int gr2 = gr;

	{
		NO_VECTORISE_NO_UNROLL
		for (sb0 = 0; sb0 <= sblimit - 1; sb0 += 1) {
			NO_VECTORISE_NO_UNROLL
			for (sample1 = 0; sample1 <= 11; sample1 += 1) {
				/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
				/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
				int _lu_fringe_1 = 2;
				NO_VECTORISE_NO_UNROLL
				for (gr2 = 0; gr2 <= 2 - _lu_fringe_1; gr2 += 2) {
					joint_sample[gr2][sample1][sb0] = 0.5 * (sb_sample[0][gr2][sample1][sb0] + sb_sample[1][gr2][sample1][sb0]);
					joint_sample[gr2 + 1][sample1][sb0] = 0.5 * (sb_sample[0][gr2 + 1][sample1][sb0] + sb_sample[1][gr2 + 1][sample1][sb0]);
				}
				NO_VECTORISE_NO_UNROLL
				for (; gr2 <= 2; gr2 += 1) {
					joint_sample[gr2][sample1][sb0] = .5 * (sb_sample[0][gr2][sample1][sb0] + sb_sample[1][gr2][sample1][sb0]);
				}
			}
		}
	}

	sb = sb0;
	sample = sample1;
	gr = gr2;
})

/**
 * @brief Run the benchmarks for encode_c_combine_lr_line330_loop_c_1_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(encode_c_combine_lr_line330_loop_c_1_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
