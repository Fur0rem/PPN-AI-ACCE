/**
 * @file codelets/codelet_libraries_default_dct_c_compute_mse_line8488_8.cpp
 * @brief Benchmark extracted from the Libraries default codelet
 */

#include "bench_common.hpp"

int i = 0;
int n = 32;
int j = 0;
double e[1 << 6][1 << 6];
int k = 0;
double basis[1 << 2 + 5 - 1][1 << 2 + 5 - 1];
double tbasis[1 << 2 + 5 - 1][1 << 2 + 5 - 1];
double autocorr[127];

OPTIMISE_SIZE
BENCHMARK_FN(dct_c_compute_mse_line8488_loop_c_8_c, {
	int i0 = i;
	int j_1 = j;
	int k2 = k;

	{
		/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
		/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
		int _lu_fringe_14 = (((n - 1 + 1 - 0) % 1 == 0 ? (n - 1 + 1 - 0) / 1 : (n - 1 + 1 - 0) / 1 + 1)) % 4 == 0 ? 0 : 4;
		for (i0 = 0; i0 <= n - 1 - _lu_fringe_14; i0 += 4) {
			for (j_1 = 0; j_1 <= n - 1; j_1 += 1) {
				// e[i0][j_1] = ((double)0);
				// for (k2 = 0; k2 <= n - 1; k2 += 1) {
				// 	e[i0][j_1] += (basis[i0][k2] - tbasis[i0][k2]) * autocorr[k2 - j_1 + 63];
				// }
				// e[i0 + 1][j_1] = ((double)0);
				// for (k2 = 0; k2 <= n - 1; k2 += 1) {
				// 	e[i0 + 1][j_1] += (basis[i0 + 1][k2] - tbasis[i0 + 1][k2]) * autocorr[k2 - j_1 + 63];
				// }
				// e[i0 + 2][j_1] = ((double)0);
				// for (k2 = 0; k2 <= n - 1; k2 += 1) {
				// 	e[i0 + 2][j_1] += (basis[i0 + 2][k2] - tbasis[i0 + 2][k2]) * autocorr[k2 - j_1 + 63];
				// }
				// e[i0 + 3][j_1] = ((double)0);
				// for (k2 = 0; k2 <= n - 1; k2 += 1) {
				// 	e[i0 + 3][j_1] += (basis[i0 + 3][k2] - tbasis[i0 + 3][k2]) * autocorr[k2 - j_1 + 63];
				// }
				for (int x = 0; x < 4; x++) {
					e[i0 + x][j_1] = ((double)0);
					for (k2 = 0; k2 <= n - 1; k2 += 1) {
						e[i0 + x][j_1] += (basis[i0 + x][k2] - tbasis[i0 + x][k2]) * autocorr[k2 - j_1 + 63];
					}
				}
			}
		}
		for (; i0 <= n - 1; i0 += 1) {
			for (j_1 = 0; j_1 <= n - 1; j_1 += 1) {
				e[i0][j_1] = ((double)0);
				for (k2 = 0; k2 <= n - 1; k2 += 1) {
					e[i0][j_1] += (basis[i0][k2] - tbasis[i0][k2]) * autocorr[k2 - j_1 + 63];
				}
			}
		}
	}

	i = i0;
	j = j_1;
	k = k2;
})

/**
 * @brief Run the benchmarks for dct_c_compute_mse_line8488_loop_c_8_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(dct_c_compute_mse_line8488_loop_c_8_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
