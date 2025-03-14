/**
 * @file benchmarks/codelet_polybench_sp_c_tzetar_line1968_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) sp_c_tzetar_line1968_0 codelet
 */

#include "bench_common.hpp"

int i = 0;
int grid_points[3] = {64, 64, 64};
int j = 0;
int k = 0;
double xvel = 0;
double us[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double yvel = 0;
double vs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double zvel = 0;
double ws[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double ac = 0;
double speed[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double acinv = 0;
double ainv[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double ac2u = 0;
double r1 = 0;
double rhs[5][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double r2 = 0;
double r3 = 0;
double r4 = 0;
double r5 = 0;
double uzik1 = 0;
double u[5][64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double btuz = 0;
double bt = 0.707107;
double t1 = 0;
double t2 = 0;
double t3 = 0;
double qs[64 / 2 * 2 + 1][64 / 2 * 2 + 1][64 / 2 * 2 + 1];
double c2iv = 2.500000;

OPTIMISE_SIZE
BENCHMARK_FN(sp_c_tzetar_line1968_loop_c_0_c, {
	int __arr_sub_0__ = grid_points[0];
	int __arr_sub_1__ = grid_points[1];
	int __arr_sub_2__ = grid_points[2];
	int i3 = i;
	int j4 = j;
	int k5 = k;

	{
		NO_VECTORISE_NO_UNROLL
		for (i3 = 1; i3 <= __arr_sub_0__ - 2; i3 += 1) {
			NO_VECTORISE_NO_UNROLL
			for (j4 = 1; j4 <= __arr_sub_1__ - 2; j4 += 1) {
				NO_VECTORISE_NO_UNROLL
				for (k5 = 1; k5 <= __arr_sub_2__ - 2; k5 += 1) {
					xvel = us[i3][j4][k5];
					yvel = vs[i3][j4][k5];
					zvel = ws[i3][j4][k5];
					ac = speed[i3][j4][k5];
					acinv = ainv[i3][j4][k5];
					ac2u = ac * ac;
					r1 = rhs[0][i3][j4][k5];
					r2 = rhs[1][i3][j4][k5];
					r3 = rhs[2][i3][j4][k5];
					r4 = rhs[3][i3][j4][k5];
					r5 = rhs[4][i3][j4][k5];
					uzik1 = u[0][i3][j4][k5];
					btuz = bt * uzik1;
					t1 = btuz * acinv * (r4 + r5);
					t2 = r3 + t1;
					t3 = btuz * (r4 - r5);
					rhs[0][i3][j4][k5] = t2;
					rhs[1][i3][j4][k5] = -uzik1 * r2 + xvel * t2;
					rhs[2][i3][j4][k5] = uzik1 * r1 + yvel * t2;
					rhs[3][i3][j4][k5] = zvel * t2 + t3;
					rhs[4][i3][j4][k5] = uzik1 * (-xvel * r2 + yvel * r1) + qs[i3][j4][k5] * t2 + c2iv * ac2u * t1 + zvel * t3;
				}
			}
		}
	}

	i = i3;
	j = j4;
	k = k5;
})

/**
 * @brief Run the benchmarks for sp_c_tzetar_line1968_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(sp_c_tzetar_line1968_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}