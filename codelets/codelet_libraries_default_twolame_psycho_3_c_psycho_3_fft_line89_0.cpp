/**
 * @file codelets/codelet_libraries_default_psycho_3_c_psycho_3_fft_line89_0.cpp
 * @brief Benchmark extracted from the Libraries default codelet
 */

#include "bench_common.hpp"

int i = 0;
double x_real[1024];
double sample[1024];
double window[1024];

BENCHMARK_FN(psycho_3_c_psycho_3_fft_line89_loop_c_0_c, {
	int __i_0__ = i;

	{
		for (__i_0__ = 0; __i_0__ <= 1023; __i_0__ += 1) {
			x_real[__i_0__] = ((double)(sample[__i_0__] * window[__i_0__]));
		}
	}

	i = __i_0__;
})

/**
 * @brief Run the benchmarks for psycho_3_c_psycho_3_fft_line89_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(psycho_3_c_psycho_3_fft_line89_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
