/**
 * @file codelets/codelet_polybench_sp_c_txinvr_line1919_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) sp_c_txinvr_line1919_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
int grid_points[3] = {64, 64, 64};
int j = 0;
int k = 0;
double ru1 = 0;
double rho_i[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double uu = 0;
double us[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double vv = 0;
double vs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double ww = 0;
double ws[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double ac = 0;
double speed[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double ac2inv = 0;
double ainv[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double r1 = 0;
double rhs[5][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double r2 = 0;
double r3 = 0;
double r4 = 0;
double r5 = 0;
double t1 = 0;
double c2 = 0.400000;
double qs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double t2 = 0;
double bt = 0.707107;
double t3 = 0;

OPTIMISE_SIZE
BENCHMARK_FN(sp_c_txinvr_line1919_loop_c_0_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[1];
	int __arr_sub_2__ = grid_points[2];
	int __i_3__ = i;
	int __j_4__ = j;
	int __k_5__ = k;

	{
		NO_VECTORISE_NO_UNROLL
		for (__i_3__ = 1; __i_3__ <= __arr_sub_0__ - 2; __i_3__ += 1) {
			NO_VECTORISE_NO_UNROLL
			for (__j_4__ = 1; __j_4__ <= __arr_sub_1__ - 2; __j_4__ += 1) {
				NO_VECTORISE_NO_UNROLL
				for (__k_5__ = 1; __k_5__ <= __arr_sub_2__ - 2; __k_5__ += 1) {
					ru1 = rho_i[__i_3__][__j_4__][__k_5__];
					uu = us[__i_3__][__j_4__][__k_5__];
					vv = vs[__i_3__][__j_4__][__k_5__];
					ww = ws[__i_3__][__j_4__][__k_5__];
					ac = speed[__i_3__][__j_4__][__k_5__];
					ac2inv = ainv[__i_3__][__j_4__][__k_5__] * ainv[__i_3__][__j_4__][__k_5__];
					r1 = rhs[0][__i_3__][__j_4__][__k_5__];
					r2 = rhs[1][__i_3__][__j_4__][__k_5__];
					r3 = rhs[2][__i_3__][__j_4__][__k_5__];
					r4 = rhs[3][__i_3__][__j_4__][__k_5__];
					r5 = rhs[4][__i_3__][__j_4__][__k_5__];
					t1 = c2 * ac2inv * (qs[__i_3__][__j_4__][__k_5__] * r1 - uu * r2 - vv * r3 - ww * r4 + r5);
					t2 = bt * ru1 * (uu * r1 - r2);
					t3 = bt * ru1 * ac * t1;
					rhs[0][__i_3__][__j_4__][__k_5__] = r1 - t1;
					rhs[1][__i_3__][__j_4__][__k_5__] = -ru1 * (ww * r1 - r4);
					rhs[2][__i_3__][__j_4__][__k_5__] = ru1 * (vv * r1 - r3);
					rhs[3][__i_3__][__j_4__][__k_5__] = -t2 + t3;
					rhs[4][__i_3__][__j_4__][__k_5__] = t2 + t3;
				}
			}
		}
	}

	i = __i_3__;
	j = __j_4__;
	k = __k_5__;
})

/**
 * @brief Run the benchmarks for sp_c_txinvr_line1919_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(sp_c_txinvr_line1919_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}