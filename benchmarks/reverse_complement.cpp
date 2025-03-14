/**
 * @file benchmarks/reverse_complement.cpp
 * @brief Benchmark for reversing a DNA sequence
 */

#include "bench_common.hpp"

char* pairs = "ATCGGCTAUAMKRYWWSSYRKMVBHDDHBVNN\n\n";
char tbl[128];

BENCHMARK_FN(reverse_complement, {
	char from[61];
	char to[61];
	for (int i = 0; i < 128; i++) {
		tbl[i] = i;
	}
	for (int i = 0; i < 60; i++) {
		from[i] = "ATCG"[i % 4];
	}
	from[60] = '\0';

	size_t len = to - from;
	size_t off = 60 - (len % 61);

	if (off) {
		char* m;
		for (m = from + 60 - off; m < to; m += 61) {
			memmove(m + 1, m, off);
			*m = '\n';
		}
	}

	for (int i = 0; i < 60; i++) {
		to[i] = tbl[from[i]];
	}
	to[60] = '\0';
})

/**
 * @brief Run the benchmarks
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(100, oss).RUN(reverse_complement).doNotOptimizeAway(tbl).doNotOptimizeAway(pairs).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
