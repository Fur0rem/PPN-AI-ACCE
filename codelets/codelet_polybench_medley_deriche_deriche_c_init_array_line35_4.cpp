/**
 * @file codelets/codelet_polybench_deriche_c_init_array_line35_4.cpp
 * @brief Benchmark extracted from the Polybench codelet
 */

#include "bench_common.hpp"

int i = 0;
int w = 4096;
int j = 0;
int h = 2160;
float img_in[4096 + 0][2160 + 0];

OPTIMISE_SIZE
BENCHMARK_FN(deriche_c_init_array_line35_loop_c_4_c, {
	int i0 = i;
	int j_1 = j;

	{
		/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
		/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
		int _lu_fringe_4 = (((w - 1 + 1 - 0) % 1 == 0 ? (w - 1 + 1 - 0) / 1 : (w - 1 + 1 - 0) / 1 + 1)) % 2 == 0 ? 0 : 2;
		for (i0 = 0; i0 <= w - 1 - _lu_fringe_4; i0 += 2) {
			for (j_1 = 0; j_1 <= h - 1; j_1 += 1) {
				img_in[i0][j_1] = ((float)((313 * i0 + 991 * j_1) % 65536)) / 65535.0f;
				img_in[i0 + 1][j_1] = ((float)((313 * (i0 + 1) + 991 * j_1) % 65536)) / 65535.0f;
			}
		}
		for (; i0 <= w - 1; i0 += 1) {
			for (j_1 = 0; j_1 <= h - 1; j_1 += 1) {
				img_in[i0][j_1] = ((float)((313 * i0 + 991 * j_1) % 65536)) / 65535.0f;
			}
		}
	}

	i = i0;
	j = j_1;
})

/**
 * @brief Run the benchmarks for deriche_c_init_array_line35_loop_c_4_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(deriche_c_init_array_line35_loop_c_4_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}