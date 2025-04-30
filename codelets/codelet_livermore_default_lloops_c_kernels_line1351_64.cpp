/**
 * @file codelets/codelet_livermore_default_lloops_c_kernels_line1351_64.cpp
 * @brief Benchmark extracted from the Livermore loops codelet
 */

#include "bench_common.hpp"

long k = 0;
long kn = 6;
long j = 0;
long jnn = 100;
double zu[7][101] = {0};
double xtra[40] = {0};
double za[7][101] = {0};
double zz[7][101] = {0};
double zb[7][101] = {0};
double zv[7][101] = {0};
double zr[7][101] = {0};

OPTIMISE_SIZE
BENCHMARK_FN(lloops_c_kernels_line1351_loop_c_64_c, {
	int k0 = k;
	int j_1 = j;

	{
		for (j_1 = 1; ((long)j_1) < jnn; j_1++) {
			for (k0 = 1; ((long)k0) < kn; k0++) {
				zu[k0][j_1] +=
					xtra[32] * (za[k0][j_1] * (zz[k0][j_1] - zz[k0][j_1 + 1]) - za[k0][j_1 - 1] * (zz[k0][j_1] - zz[k0][j_1 - 1]) -
								zb[k0][j_1] * (zz[k0][j_1] - zz[k0 - 1][j_1]) + zb[k0 + 1][j_1] * (zz[k0][j_1] - zz[k0 + 1][j_1]));
				zv[k0][j_1] +=
					xtra[32] * (za[k0][j_1] * (zr[k0][j_1] - zr[k0][j_1 + 1]) - za[k0][j_1 - 1] * (zr[k0][j_1] - zr[k0][j_1 - 1]) -
								zb[k0][j_1] * (zr[k0][j_1] - zr[k0 - 1][j_1]) + zb[k0 + 1][j_1] * (zr[k0][j_1] - zr[k0 + 1][j_1]));
			}
		}
	}

	k = k0;
	j = j_1;
})

/**
 * @brief Run the benchmarks for lloops_c_kernels_line1351_main_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss)
					  .RUN(lloops_c_kernels_line1351_loop_c_64_c)
					  .doNotOptimizeAway(zu)
					  .doNotOptimizeAway(xtra)
					  .doNotOptimizeAway(za)
					  .doNotOptimizeAway(zz)
					  .results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
