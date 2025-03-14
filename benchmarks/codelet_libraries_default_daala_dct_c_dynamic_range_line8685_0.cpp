/**
 * @file benchmarks/codelet_libraries_default_dct_c_dynamic_range_line8685_0.cpp
 * @brief Benchmark extracted from the Libraries default codelet
 */

#include "bench_common.hpp"

int u = 0;
int n = 64;
int v = 0;
int x[64 * 2][64 * 2];
int i = 72;
int j = 55;
typedef int32_t od_coeff;

BENCHMARK_FN(dct_c_dynamic_range_line8685_loop_c_0_c, {
	int __u_0__ = u;
	int __v_1__ = v;

	{
		for (__u_0__ = 0; __u_0__ <= n * 2 - 1; __u_0__ += 1) {
			for (__v_1__ = 0; __v_1__ <= n * 2 - 1; __v_1__ += 1) {
				x[__u_0__][__v_1__] = (__u_0__ == i && __v_1__ == j) << 8 + 4;
			}
		}
	}

	u = __u_0__;
	v = __v_1__;
})

/**
 * @brief Run the benchmarks for dct_c_dynamic_range_line8685_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(dct_c_dynamic_range_line8685_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
