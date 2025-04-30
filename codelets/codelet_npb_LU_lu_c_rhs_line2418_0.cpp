/**
 * @file codelets/codelet_polybench_lu_c_rhs_line2418_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) lu_c_rhs_line2418_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
int ist = 1;
int iend = 62;
int j = 0;
int L1 = 0;
int L2 = 63;
int k = 0;
int nz = 64;
double flux[64][64 / 2 * 2 + 1][64 / 2 * 2 + 1][5];
double u[64][64 / 2 * 2 + 1][64 / 2 * 2 + 1][5];
double u31 = 0;
double q = 0;

BENCHMARK_FN(lu_c_rhs_line2418_loop_c_0_c, {
	int __i_0__ = i;
	int __j_1__ = j;
	int __k_2__ = k;

	{
		for (__i_0__ = ist; __i_0__ <= iend; __i_0__ += 1) {
			for (__j_1__ = L1; __j_1__ <= L2; __j_1__ += 1) {
				for (__k_2__ = 1; __k_2__ <= nz - 2; __k_2__ += 1) {
					flux[__i_0__][__j_1__][__k_2__][0] = u[__i_0__][__j_1__][__k_2__][2];
					u31 = u[__i_0__][__j_1__][__k_2__][2] / u[__i_0__][__j_1__][__k_2__][0];
					q = 0.50 *
						(u[__i_0__][__j_1__][__k_2__][1] * u[__i_0__][__j_1__][__k_2__][1] +
						 u[__i_0__][__j_1__][__k_2__][2] * u[__i_0__][__j_1__][__k_2__][2] +
						 u[__i_0__][__j_1__][__k_2__][3] * u[__i_0__][__j_1__][__k_2__][3]) /
						u[__i_0__][__j_1__][__k_2__][0];
					flux[__i_0__][__j_1__][__k_2__][1] = u[__i_0__][__j_1__][__k_2__][1] * u31;
					flux[__i_0__][__j_1__][__k_2__][2] =
						u[__i_0__][__j_1__][__k_2__][2] * u31 + 0.40e+00 * (u[__i_0__][__j_1__][__k_2__][4] - q);
					flux[__i_0__][__j_1__][__k_2__][3] = u[__i_0__][__j_1__][__k_2__][3] * u31;
					flux[__i_0__][__j_1__][__k_2__][4] = (1.40e+00 * u[__i_0__][__j_1__][__k_2__][4] - 0.40e+00 * q) * u31;
				}
			}
		}
	}

	i = __i_0__;
	j = __j_1__;
	k = __k_2__;
})

/**
 * @brief Run the benchmarks for lu_c_rhs_line2418_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(lu_c_rhs_line2418_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}