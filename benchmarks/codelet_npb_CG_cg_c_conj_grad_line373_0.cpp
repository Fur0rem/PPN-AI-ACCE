/**
 * @file benchmarks/codelet_polybench_cg_c_conj_grad_line373_0.cpp
 * @brief Benchmark extracted from the NAS Parallel Benchmarks (NPB) cg_c_conj_grad_line373_0 codelet
 */

#include "bench_common.hpp"

int j = 0;
constexpr int NAA = 14000;
double x[NAA + 2];
double q[NAA + 2];
double z[NAA + 2];
double r[NAA + 2];
double p[NAA + 2];
double w[NAA + 2];

BENCHMARK_FN(cg_c_conj_grad_line373_loop_c_0_c, {
	int __j_0__ = j;
	double __xj_1__;

	{
		for (__j_0__ = 1; __j_0__ <= NAA + 1; __j_0__ += 1) {
			__xj_1__ = x[__j_0__];
			q[__j_0__] = 0.0;
			z[__j_0__] = 0.0;
			r[__j_0__] = __xj_1__;
			p[__j_0__] = __xj_1__;
			w[__j_0__] = 0.0;
		}
	}

	j = __j_0__;
})

void init_arrays() {
	for (int i = 0; i < NAA; i++) {
		x[i] = 1.0;
		q[i] = 0.0;
		z[i] = 0.0;
		r[i] = 1.0;
		p[i] = 1.0;
		w[i] = 0.0;
	}
}

/**
 * @brief Run the benchmarks for cg_c_conj_grad_line373_loop_c_0_c
 */
int main() {
	init_arrays();
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(cg_c_conj_grad_line373_loop_c_0_c).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}