/**
 * @file benchmarks/codelet_netlib.cpp
 * @brief Benchmark extracted from the NETLIB codelet
 */

#include "bench_common.hpp"

constexpr int N = 9841; ///< Number of elements
volatile double r[N];
volatile double rnrm2;

BENCHMARK_FN(cg_rc_prb_test02_line432_0, {
	for (int i = 0; i < N; ++i) {
		rnrm2 = rnrm2 + r[i] * r[i];
	}
})

volatile double ax[N];

BENCHMARK_FN(cg_rc_prb_test02_line421_0, {
	for (int i = 0; i < N; ++i) {
		r[i] = r[i] - ax[i];
	}
})

void init() {
	srand(time(nullptr));
	for (int i = 0; i < N; ++i) {
		r[i] = rand() / (double)RAND_MAX;
		ax[i] = rand() / (double)RAND_MAX;
	}
	rnrm2 = 0.0;
}

constexpr int N2 = 5000;
volatile double x[N2];
double a = -1.000000;
double b = 1.000000;

BENCHMARK_FN(chebyshev_prb_c_test01_line115, {
	for (int i = 0; i < N2; ++i) {
		x[i] = 0.5 * (a + b) + x[i] * 0.5 * (b - a);
	}
})

BENCHMARK_FN(chebyshev_prb_c_test01_line115_rev, {
	for (int i = N2 - 1; i > 0; --i) {
		x[i] = 0.5 * (a + b) + x[i] * 0.5 * (b - a);
	}
})

constexpr int N3 = 1200000;
double e1[5];
constexpr double T = 0.499975;

BENCHMARK_FN(whetstone_c_main_line187_0, {
	for (int i = 1; i <= N3; ++i) {
		e1[1] = (e1[1] + e1[2] + e1[3] - e1[4]) * T;
		e1[2] = (e1[1] + e1[2] - e1[3] + e1[4]) * T;
		e1[3] = (e1[1] - e1[2] + e1[3] + e1[4]) * T;
		e1[4] = (-e1[1] + e1[2] + e1[3] + e1[4]) * T;
	}
})

BENCHMARK_FN(whetstone_c_main_line187_1, {
	int lu_fringe_1 = (((N3 + 1 - 1) % 1 == 0 ? (N3 + 1 - 1) / 1 : (N3 + 1 - 1) / 1 + 1)) % 2 == 0 ? 0 : 2;
	int i;
	for (i = 1; i <= N3 - lu_fringe_1; i += 2) {
		e1[1] = (e1[1] + e1[2] + e1[3] - e1[4]) * T;
		e1[2] = (e1[1] + e1[2] - e1[3] + e1[4]) * T;
		e1[3] = (e1[1] - e1[2] + e1[3] + e1[4]) * T;
		e1[4] = (-e1[1] + e1[2] + e1[3] + e1[4]) * T;
		e1[1] = (e1[1] + e1[2] + e1[3] - e1[4]) * T;
		e1[2] = (e1[1] + e1[2] - e1[3] + e1[4]) * T;
		e1[3] = (e1[1] - e1[2] + e1[3] + e1[4]) * T;
		e1[4] = (-e1[1] + e1[2] + e1[3] + e1[4]) * T;
	}
	for (; i <= N3; i += 1) {
		e1[1] = (e1[1] + e1[2] + e1[3] - e1[4]) * T;
		e1[2] = (e1[1] + e1[2] - e1[3] + e1[4]) * T;
		e1[3] = (e1[1] - e1[2] + e1[3] + e1[4]) * T;
		e1[4] = (-e1[1] + e1[2] + e1[3] + e1[4]) * T;
	}
})

/**
 * @brief Run the benchmarks for netlib
 */
int main() {
	init();
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss)
					  .RUN(cg_rc_prb_test02_line432_0)
					  .RUN(cg_rc_prb_test02_line421_0)
					  .RUN(chebyshev_prb_c_test01_line115)
					  .RUN(chebyshev_prb_c_test01_line115_rev)
					  .RUN(whetstone_c_main_line187_0)
					  .RUN(whetstone_c_main_line187_1)
					  .doNotOptimizeAway(rnrm2)
					  .doNotOptimizeAway(r)
					  .doNotOptimizeAway(ax)
					  .doNotOptimizeAway(x)
					  .doNotOptimizeAway(a)
					  .doNotOptimizeAway(b)
					  .doNotOptimizeAway(N)
					  .doNotOptimizeAway(N2)
					  .results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
