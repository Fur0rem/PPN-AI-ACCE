/**
 * @file codelets/codelet_livermore_default_lloops_c_kernels_line1113_3.cpp
 * @brief Benchmark extracted from the Livermore loops codelet
 */

#include "bench_common.hpp"

long k = 0;
long inner_loops = 1001;
double vx[1001] = {0};
double ex1[1001] = {0};
double xx[1001] = {0};
double xi[1001] = {0};
double dex1[1001] = {0};
double xtra[40] = {0};
long ir[1001] = {0};
double rx[1001] = {0};

OPTIMISE_SIZE
BENCHMARK_FN(lloops_c_kernels_line1113_loop_c_3_c, {
	int k0 = k;

	{
		/* iter_count = (ub-lb+1)%step ==0?(ub-lb+1)/step: (ub-lb+1)/step+1; */
		/* fringe = iter_count%unroll_factor==0 ? 0:unroll_factor*step */
		int _lu_fringe_3 =
			(((inner_loops - 1 + 1 - 0) % 1 == 0 ? (inner_loops - 1 + 1 - 0) / 1 : (inner_loops - 1 + 1 - 0) / 1 + 1)) % 8 == 0 ? 0 : 8;
		for (k0 = 0; ((long)k0) <= inner_loops - 1 - _lu_fringe_3; k0 += 8) {
			// vx[k0] = vx[k0] + ex1[k0] + (xx[k0] - xi[k0]) * dex1[k0];
			// xx[k0] = xx[k0] + vx[k0] + xtra[27];
			// ir[k0] = ((long)xx[k0]);
			// rx[k0] = xx[k0] - ((double)ir[k0]);
			// ir[k0] = (ir[k0] & ((long)2047)) + ((long)1);
			// xx[k0] = rx[k0] + ((double)ir[k0]);
			// {
			// 	vx[k0 + 1] = vx[k0 + 1] + ex1[k0 + 1] + (xx[k0 + 1] - xi[k0 + 1]) * dex1[k0 + 1];
			// 	xx[k0 + 1] = xx[k0 + 1] + vx[k0 + 1] + xtra[27];
			// 	ir[k0 + 1] = ((long)xx[k0 + 1]);
			// 	rx[k0 + 1] = xx[k0 + 1] - ((double)ir[k0 + 1]);
			// 	ir[k0 + 1] = (ir[k0 + 1] & ((long)2047)) + ((long)1);
			// 	xx[k0 + 1] = rx[k0 + 1] + ((double)ir[k0 + 1]);
			// }
			// {
			// 	vx[k0 + 2] = vx[k0 + 2] + ex1[k0 + 2] + (xx[k0 + 2] - xi[k0 + 2]) * dex1[k0 + 2];
			// 	xx[k0 + 2] = xx[k0 + 2] + vx[k0 + 2] + xtra[27];
			// 	ir[k0 + 2] = ((long)xx[k0 + 2]);
			// 	rx[k0 + 2] = xx[k0 + 2] - ((double)ir[k0 + 2]);
			// 	ir[k0 + 2] = (ir[k0 + 2] & ((long)2047)) + ((long)1);
			// 	xx[k0 + 2] = rx[k0 + 2] + ((double)ir[k0 + 2]);
			// }
			// {
			// 	vx[k0 + 3] = vx[k0 + 3] + ex1[k0 + 3] + (xx[k0 + 3] - xi[k0 + 3]) * dex1[k0 + 3];
			// 	xx[k0 + 3] = xx[k0 + 3] + vx[k0 + 3] + xtra[27];
			// 	ir[k0 + 3] = ((long)xx[k0 + 3]);
			// 	rx[k0 + 3] = xx[k0 + 3] - ((double)ir[k0 + 3]);
			// 	ir[k0 + 3] = (ir[k0 + 3] & ((long)2047)) + ((long)1);
			// 	xx[k0 + 3] = rx[k0 + 3] + ((double)ir[k0 + 3]);
			// }
			// {
			// 	vx[k0 + 4] = vx[k0 + 4] + ex1[k0 + 4] + (xx[k0 + 4] - xi[k0 + 4]) * dex1[k0 + 4];
			// 	xx[k0 + 4] = xx[k0 + 4] + vx[k0 + 4] + xtra[27];
			// 	ir[k0 + 4] = ((long)xx[k0 + 4]);
			// 	rx[k0 + 4] = xx[k0 + 4] - ((double)ir[k0 + 4]);
			// 	ir[k0 + 4] = (ir[k0 + 4] & ((long)2047)) + ((long)1);
			// 	xx[k0 + 4] = rx[k0 + 4] + ((double)ir[k0 + 4]);
			// }
			// {
			// 	vx[k0 + 5] = vx[k0 + 5] + ex1[k0 + 5] + (xx[k0 + 5] - xi[k0 + 5]) * dex1[k0 + 5];
			// 	xx[k0 + 5] = xx[k0 + 5] + vx[k0 + 5] + xtra[27];
			// 	ir[k0 + 5] = ((long)xx[k0 + 5]);
			// 	rx[k0 + 5] = xx[k0 + 5] - ((double)ir[k0 + 5]);
			// 	ir[k0 + 5] = (ir[k0 + 5] & ((long)2047)) + ((long)1);
			// 	xx[k0 + 5] = rx[k0 + 5] + ((double)ir[k0 + 5]);
			// }
			// {
			// 	vx[k0 + 6] = vx[k0 + 6] + ex1[k0 + 6] + (xx[k0 + 6] - xi[k0 + 6]) * dex1[k0 + 6];
			// 	xx[k0 + 6] = xx[k0 + 6] + vx[k0 + 6] + xtra[27];
			// 	ir[k0 + 6] = ((long)xx[k0 + 6]);
			// 	rx[k0 + 6] = xx[k0 + 6] - ((double)ir[k0 + 6]);
			// 	ir[k0 + 6] = (ir[k0 + 6] & ((long)2047)) + ((long)1);
			// 	xx[k0 + 6] = rx[k0 + 6] + ((double)ir[k0 + 6]);
			// }
			// {
			// 	vx[k0 + 7] = vx[k0 + 7] + ex1[k0 + 7] + (xx[k0 + 7] - xi[k0 + 7]) * dex1[k0 + 7];
			// 	xx[k0 + 7] = xx[k0 + 7] + vx[k0 + 7] + xtra[27];
			// 	ir[k0 + 7] = ((long)xx[k0 + 7]);
			// 	rx[k0 + 7] = xx[k0 + 7] - ((double)ir[k0 + 7]);
			// 	ir[k0 + 7] = (ir[k0 + 7] & ((long)2047)) + ((long)1);
			// 	xx[k0 + 7] = rx[k0 + 7] + ((double)ir[k0 + 7]);
			// }
			for (int x = 0; x < 8; x++) {
				vx[k0 + x] = vx[k0 + x] + ex1[k0 + x] + (xx[k0 + x] - xi[k0 + x]) * dex1[k0 + x];
				xx[k0 + x] = xx[k0 + x] + vx[k0 + x] + xtra[27];
				ir[k0 + x] = ((long)xx[k0 + x]);
				rx[k0 + x] = xx[k0 + x] - ((double)ir[k0 + x]);
				ir[k0 + x] = (ir[k0 + x] & ((long)2047)) + ((long)1);
				xx[k0 + x] = rx[k0 + x] + ((double)ir[k0 + x]);
			}
		}
		for (; ((long)k0) <= inner_loops - 1; k0 += 1) {
			vx[k0] = vx[k0] + ex1[k0] + (xx[k0] - xi[k0]) * dex1[k0];
			xx[k0] = xx[k0] + vx[k0] + xtra[27];
			ir[k0] = ((long)xx[k0]);
			rx[k0] = xx[k0] - ((double)ir[k0]);
			ir[k0] = (ir[k0] & ((long)2047)) + ((long)1);
			xx[k0] = rx[k0] + ((double)ir[k0]);
		}
	}

	k = k0;
})

/**
 * @brief Run the benchmarks for lloops_c_kernels_line1113_loop_c_3_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(lloops_c_kernels_line1113_loop_c_3_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
