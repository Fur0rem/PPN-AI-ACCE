/**
 * @file codelets/codelet_polybench_diffuse_c_diffusion_line142_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) diffuse_c_diffusion_line142_0 codelet
 */

#include "bench_common.hpp"

int ie = 0;
int nelt = 2003;
int k = 0;
int j = 0;
int i = 0;
double cona = 0.000000;
double pdiff[2400][5][5][5];
double pdiffp[2400][5][5][5];
double tmult[2400][5][5][5];

OPTIMISE_SIZE
BENCHMARK_FN(diffuse_c_diffusion_line142_loop_c_0_c, {
	int __ie_0__ = ie;
	int __k_1__ = k;
	int __j_2__ = j;
	int __i_3__ = i;

	{
		for (__ie_0__ = 0; __ie_0__ <= nelt - 1; __ie_0__ += 1) {
			for (__k_1__ = 0; __k_1__ <= 4; __k_1__ += 1) {
				for (__j_2__ = 0; __j_2__ <= 4; __j_2__ += 1) {
					for (__i_3__ = 0; __i_3__ <= 4; __i_3__ += 1) {
						cona = cona + pdiff[__ie_0__][__k_1__][__j_2__][__i_3__] * pdiffp[__ie_0__][__k_1__][__j_2__][__i_3__] *
										  tmult[__ie_0__][__k_1__][__j_2__][__i_3__];
					}
				}
			}
		}
	}

	ie = __ie_0__;
	k = __k_1__;
	j = __j_2__;
	i = __i_3__;
})

/**
 * @brief Run the benchmarks for diffuse_c_diffusion_line142_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(diffuse_c_diffusion_line142_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}