/**
 * @file codelets/codelet_crystal_cholesky.cpp
 * @brief Crystal Cholesky benchmark for a loop
 */

#include "bench_common.hpp"

constexpr int NB_SLIP = 100;
double a[NB_SLIP][NB_SLIP] = {5.0};

BENCHMARK_FN(codelet_crystal_cholesky_line29_loop_0, {
	for (int i = 1; i <= NB_SLIP - 1; i += 1) {
		a[i][0] = a[i][0] / a[0][0];
	}
})

BENCHMARK_FN(codelet_crystal_cholesky_line29_loop_1, {
	int lu_frige_1 = (((NB_SLIP - 1 + 1 - 1) % 1 == 0 ? (NB_SLIP - 1 + 1 - 1) / 1 : (NB_SLIP - 1 + 1 - 1) / 1 + 1)) % 2 == 0 ? 0 : 2;
	for (int i = 1; i <= NB_SLIP - 1 - lu_frige_1; i += 2) {
		a[i][0] = a[i][0] / a[0][0];
		a[i + 1][0] = a[i + 1][0] / a[0][0];
	}
	for (int i = NB_SLIP - 1; i <= NB_SLIP - 1; i += 1) {
		a[i][0] = a[i][0] / a[0][0];
	}
})

double sgn[NB_SLIP] = {0.0};
double rate_fact[NB_SLIP] = {0.0};

BENCHMARK_FN(codelet_crystal_div_line43_loop, {
	for (int n = 0; n <= NB_SLIP - 1; n += 1) {
		sgn[n] = 1.0;
		rate_fact[n] = 0.9 + 0.2 * n / 100;
	}
})

double bor_array[NB_SLIP] = {0.0};
double slip_rate[NB_SLIP] = {0.0};
double rate_offset = 0.000001;

BENCHMARK_FN(codelet_crystal_div_line49_loop, {
	for (int n = 0; n <= NB_SLIP - 1; n += 1) {
		bor_array[n] = 1 / (slip_rate[n] * sgn[n] + rate_offset);
	}
})

double dtcdgd[NB_SLIP][NB_SLIP] = {10.0};

BENCHMARK_FN(codelet_crystal_div_line61_loop, {
	for (int n = 0; n <= NB_SLIP - 1; n += 1) {
		bor_array[n] = 1 / dtcdgd[n][n];
	}
})

/**
 * @brief Run the benchmarks for codelet_crystal_cholesky
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(1000, oss)
					  .RUN(codelet_crystal_cholesky_line29_loop_0)
					  .RUN(codelet_crystal_cholesky_line29_loop_1)
					  .RUN(codelet_crystal_div_line43_loop)
					  .RUN(codelet_crystal_div_line49_loop)
					  .RUN(codelet_crystal_div_line61_loop)
					  .doNotOptimizeAway(a)
					  .doNotOptimizeAway(sgn)
					  .doNotOptimizeAway(rate_fact)
					  .doNotOptimizeAway(bor_array)
					  .doNotOptimizeAway(slip_rate)
					  .doNotOptimizeAway(dtcdgd)
					  .doNotOptimizeAway(rate_offset)
					  .doNotOptimizeAway(NB_SLIP)
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
