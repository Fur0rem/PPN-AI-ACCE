/**
 * @file codelets/codelet_polybench_adapt_c_remap_line894_306.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) adapt_c_remap_line894_306 codelet
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

OPTIMISE_SIZE
BENCHMARK_FN(adapt_c_remap_line894_loop_c_306_c, {
	int iz0 = iz;
	int kk1 = kk;
	int jj2 = jj;
	int ii3 = ii;

	{
		for (jj2 = 0; jj2 <= 4; jj2 += 1) {
			for (iz0 = 0; iz0 <= 4; iz0 += 1) {
				/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
				/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
				int _lu_fringe_812 = 4;
				for (kk1 = 0; kk1 <= 4 - _lu_fringe_812; kk1 += 4) {
					for (ii3 = 0; ii3 <= 4; ii3 += 1) {
						// y[jj2][iz0][ii3] = y[jj2][iz0][ii3] + ytwo[0][iz0][kk1][ii3] * ixtmc1[jj2][kk1];
						// y_1[0][jj2][iz0][ii3] = y_1[0][jj2][iz0][ii3] + ytwo[2][iz0][kk1][ii3] * ixtmc1[jj2][kk1];
						// y_1[1][jj2][iz0][ii3] = y_1[1][jj2][iz0][ii3] + ytwo[1][iz0][kk1][ii3] * ixtmc1[jj2][kk1];
						// y_1[2][jj2][iz0][ii3] = y_1[2][jj2][iz0][ii3] + ytwo[3][iz0][kk1][ii3] * ixtmc1[jj2][kk1];
						// y_1[3][jj2][iz0][ii3] = y_1[3][jj2][iz0][ii3] + ytwo[0][iz0][kk1][ii3] * ixtmc2[jj2][kk1];
						// y_1[4][jj2][iz0][ii3] = y_1[4][jj2][iz0][ii3] + ytwo[2][iz0][kk1][ii3] * ixtmc2[jj2][kk1];
						// y_1[5][jj2][iz0][ii3] = y_1[5][jj2][iz0][ii3] + ytwo[1][iz0][kk1][ii3] * ixtmc2[jj2][kk1];
						// y_1[6][jj2][iz0][ii3] = y_1[6][jj2][iz0][ii3] + ytwo[3][iz0][kk1][ii3] * ixtmc2[jj2][kk1];
						// y[jj2][iz0][ii3] = y[jj2][iz0][ii3] + ytwo[0][iz0][kk1 + 1][ii3] * ixtmc1[jj2][kk1 + 1];
						// y_1[0][jj2][iz0][ii3] = y_1[0][jj2][iz0][ii3] + ytwo[2][iz0][kk1 + 1][ii3] * ixtmc1[jj2][kk1 + 1];
						// y_1[1][jj2][iz0][ii3] = y_1[1][jj2][iz0][ii3] + ytwo[1][iz0][kk1 + 1][ii3] * ixtmc1[jj2][kk1 + 1];
						// y_1[2][jj2][iz0][ii3] = y_1[2][jj2][iz0][ii3] + ytwo[3][iz0][kk1 + 1][ii3] * ixtmc1[jj2][kk1 + 1];
						// y_1[3][jj2][iz0][ii3] = y_1[3][jj2][iz0][ii3] + ytwo[0][iz0][kk1 + 1][ii3] * ixtmc2[jj2][kk1 + 1];
						// y_1[4][jj2][iz0][ii3] = y_1[4][jj2][iz0][ii3] + ytwo[2][iz0][kk1 + 1][ii3] * ixtmc2[jj2][kk1 + 1];
						// y_1[5][jj2][iz0][ii3] = y_1[5][jj2][iz0][ii3] + ytwo[1][iz0][kk1 + 1][ii3] * ixtmc2[jj2][kk1 + 1];
						// y_1[6][jj2][iz0][ii3] = y_1[6][jj2][iz0][ii3] + ytwo[3][iz0][kk1 + 1][ii3] * ixtmc2[jj2][kk1 + 1];
						// y[jj2][iz0][ii3] = y[jj2][iz0][ii3] + ytwo[0][iz0][kk1 + 2][ii3] * ixtmc1[jj2][kk1 + 2];
						// y_1[0][jj2][iz0][ii3] = y_1[0][jj2][iz0][ii3] + ytwo[2][iz0][kk1 + 2][ii3] * ixtmc1[jj2][kk1 + 2];
						// y_1[1][jj2][iz0][ii3] = y_1[1][jj2][iz0][ii3] + ytwo[1][iz0][kk1 + 2][ii3] * ixtmc1[jj2][kk1 + 2];
						// y_1[2][jj2][iz0][ii3] = y_1[2][jj2][iz0][ii3] + ytwo[3][iz0][kk1 + 2][ii3] * ixtmc1[jj2][kk1 + 2];
						// y_1[3][jj2][iz0][ii3] = y_1[3][jj2][iz0][ii3] + ytwo[0][iz0][kk1 + 2][ii3] * ixtmc2[jj2][kk1 + 2];
						// y_1[4][jj2][iz0][ii3] = y_1[4][jj2][iz0][ii3] + ytwo[2][iz0][kk1 + 2][ii3] * ixtmc2[jj2][kk1 + 2];
						// y_1[5][jj2][iz0][ii3] = y_1[5][jj2][iz0][ii3] + ytwo[1][iz0][kk1 + 2][ii3] * ixtmc2[jj2][kk1 + 2];
						// y_1[6][jj2][iz0][ii3] = y_1[6][jj2][iz0][ii3] + ytwo[3][iz0][kk1 + 2][ii3] * ixtmc2[jj2][kk1 + 2];
						// y[jj2][iz0][ii3] = y[jj2][iz0][ii3] + ytwo[0][iz0][kk1 + 3][ii3] * ixtmc1[jj2][kk1 + 3];
						// y_1[0][jj2][iz0][ii3] = y_1[0][jj2][iz0][ii3] + ytwo[2][iz0][kk1 + 3][ii3] * ixtmc1[jj2][kk1 + 3];
						// y_1[1][jj2][iz0][ii3] = y_1[1][jj2][iz0][ii3] + ytwo[1][iz0][kk1 + 3][ii3] * ixtmc1[jj2][kk1 + 3];
						// y_1[2][jj2][iz0][ii3] = y_1[2][jj2][iz0][ii3] + ytwo[3][iz0][kk1 + 3][ii3] * ixtmc1[jj2][kk1 + 3];
						// y_1[3][jj2][iz0][ii3] = y_1[3][jj2][iz0][ii3] + ytwo[0][iz0][kk1 + 3][ii3] * ixtmc2[jj2][kk1 + 3];
						// y_1[4][jj2][iz0][ii3] = y_1[4][jj2][iz0][ii3] + ytwo[2][iz0][kk1 + 3][ii3] * ixtmc2[jj2][kk1 + 3];
						// y_1[5][jj2][iz0][ii3] = y_1[5][jj2][iz0][ii3] + ytwo[1][iz0][kk1 + 3][ii3] * ixtmc2[jj2][kk1 + 3];
						// y_1[6][jj2][iz0][ii3] = y_1[6][jj2][iz0][ii3] + ytwo[3][iz0][kk1 + 3][ii3] * ixtmc2[jj2][kk1 + 3];
						for (int x = 0; x < 4; x++) {
							y[jj2][iz0][ii3] = y[jj2][iz0][ii3] + ytwo[0][iz0][kk1 + x][ii3] * ixtmc1[jj2][kk1 + x];
							y_1[0][jj2][iz0][ii3] = y_1[0][jj2][iz0][ii3] + ytwo[2][iz0][kk1 + x][ii3] * ixtmc1[jj2][kk1 + x];
							y_1[1][jj2][iz0][ii3] = y_1[1][jj2][iz0][ii3] + ytwo[1][iz0][kk1 + x][ii3] * ixtmc1[jj2][kk1 + x];
							y_1[2][jj2][iz0][ii3] = y_1[2][jj2][iz0][ii3] + ytwo[3][iz0][kk1 + x][ii3] * ixtmc1[jj2][kk1 + x];
							y_1[3][jj2][iz0][ii3] = y_1[3][jj2][iz0][ii3] + ytwo[0][iz0][kk1 + x][ii3] * ixtmc2[jj2][kk1 + x];
							y_1[4][jj2][iz0][ii3] = y_1[4][jj2][iz0][ii3] + ytwo[2][iz0][kk1 + x][ii3] * ixtmc2[jj2][kk1 + x];
							y_1[5][jj2][iz0][ii3] = y_1[5][jj2][iz0][ii3] + ytwo[1][iz0][kk1 + x][ii3] * ixtmc2[jj2][kk1 + x];
							y_1[6][jj2][iz0][ii3] = y_1[6][jj2][iz0][ii3] + ytwo[3][iz0][kk1 + x][ii3] * ixtmc2[jj2][kk1 + x];
							y[jj2][iz0][ii3] = y[jj2][iz0][ii3] + ytwo[0][iz0][kk1 + x][ii3] * ixtmc1[jj2][kk1 + x];
						}
					}
				}
				for (; kk1 <= 4; kk1 += 1) {
					for (ii3 = 0; ii3 <= 4; ii3 += 1) {
						y[jj2][iz0][ii3] = y[jj2][iz0][ii3] + ytwo[0][iz0][kk1][ii3] * ixtmc1[jj2][kk1];
						y_1[0][jj2][iz0][ii3] = y_1[0][jj2][iz0][ii3] + ytwo[2][iz0][kk1][ii3] * ixtmc1[jj2][kk1];
						y_1[1][jj2][iz0][ii3] = y_1[1][jj2][iz0][ii3] + ytwo[1][iz0][kk1][ii3] * ixtmc1[jj2][kk1];
						y_1[2][jj2][iz0][ii3] = y_1[2][jj2][iz0][ii3] + ytwo[3][iz0][kk1][ii3] * ixtmc1[jj2][kk1];
						y_1[3][jj2][iz0][ii3] = y_1[3][jj2][iz0][ii3] + ytwo[0][iz0][kk1][ii3] * ixtmc2[jj2][kk1];
						y_1[4][jj2][iz0][ii3] = y_1[4][jj2][iz0][ii3] + ytwo[2][iz0][kk1][ii3] * ixtmc2[jj2][kk1];
						y_1[5][jj2][iz0][ii3] = y_1[5][jj2][iz0][ii3] + ytwo[1][iz0][kk1][ii3] * ixtmc2[jj2][kk1];
						y_1[6][jj2][iz0][ii3] = y_1[6][jj2][iz0][ii3] + ytwo[3][iz0][kk1][ii3] * ixtmc2[jj2][kk1];
					}
				}
			}
		}
	}

	iz = iz0;
	kk = kk1;
	jj = jj2;
	ii = ii3;
})

/**
 * @brief Run the benchmarks for adapt_c_remap_line894_loop_c_306_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(adapt_c_remap_line894_loop_c_306_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}