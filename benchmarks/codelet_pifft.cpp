/**
 * @file benchmarks/codelet_pifft.cpp
 * @brief Benchmark for pifft
 */

#include "bench_common.hpp"

constexpr int N = 1048578;
int shift = 1;
volatile double* in1;
volatile double* out;
double dradix = 10000.000000;
int carry = 148470;
double d1_in2 = 0.000001;
double din2 = 1048576.000000;

BENCHMARK_FN(codelet_pifft, {
	for (int j = 2; j <= N - shift; j += 1) {
		double x = in1[j + shift] + dradix * carry + 0.5;
		int ix = (int)(d1_in2 * x);
		carry = (int)(x - din2 * ix);
		out[j] = ix;
	}
})

void init() {
	in1 = new double[N + shift];
	out = new double[N + shift];
	srand(time(nullptr));
	for (int i = 0; i < N + shift; i++) {
		in1[i] = rand() / (double)RAND_MAX;
	}
}

/**
 * @brief Run the benchmarks for pifft
 */
int main() {
	init();
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(10, oss).RUN(codelet_pifft).doNotOptimizeAway(out).doNotOptimizeAway(in1).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
