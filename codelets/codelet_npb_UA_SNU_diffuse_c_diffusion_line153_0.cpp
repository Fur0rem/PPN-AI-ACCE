/**
 * @file codelets/codelet_polybench_diffuse_c_diffusion_line153_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) diffuse_c_diffusion_line153_0 codelet
 */

#include "bench_common.hpp"

int im = 0;
int nmor = 85456;
double ppmor[92700];
double tmmor[92700];
double cona = 0.000000;
double pmorx[92700];

BENCHMARK_FN(diffuse_c_diffusion_line153_loop_c_0_c, {
	int __im_0__ = im;

	{
		for (__im_0__ = 0; __im_0__ <= nmor - 1; __im_0__ += 1) {
			ppmor[__im_0__] = ppmor[__im_0__] * tmmor[__im_0__];
			cona = cona + pmorx[__im_0__] * ppmor[__im_0__];
		}
	}

	im = __im_0__;
})

/**
 * @brief Run the benchmarks for diffuse_c_diffusion_line153_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(diffuse_c_diffusion_line153_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}