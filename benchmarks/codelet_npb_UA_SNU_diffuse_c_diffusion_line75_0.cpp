/**
 * @file benchmarks/codelet_polybench_diffuse_c_diffusion_line75_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) diffuse_c_diffusion_line75_0 codelet
 */

#include "bench_common.hpp"

int im = 0;
int nmor = 85456;
double pmorx[92700];
double dpcmor[92700];
double rmor[92700];
double rho1 = 0.000000;

BENCHMARK_FN(diffuse_c_diffusion_line75_loop_c_0_c, {
	int __im_0__ = im;

	{
		for (__im_0__ = 0; __im_0__ <= nmor - 1; __im_0__ += 1) {
			pmorx[__im_0__] = dpcmor[__im_0__] * rmor[__im_0__];
			rho1 = rho1 + rmor[__im_0__] * pmorx[__im_0__];
		}
	}

	im = __im_0__;
})

/**
 * @brief Run the benchmarks for diffuse_c_diffusion_line75_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(diffuse_c_diffusion_line75_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}