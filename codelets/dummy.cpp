/**
 * @file codelets/dummy.cpp
 * @brief Dummy benchmark as a template for new benchmarks
 */

#include "bench_common.hpp"

void dummy() {}

/**
 * @brief Run the benchmarks for dummy
 */
int main() {
	std::ostringstream oss;
	auto result = ankerl::nanobench::Bench().minEpochIterations(1000).performanceCounters(true).output(&oss).run("dummy", dummy).results();

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
