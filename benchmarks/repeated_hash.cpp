/**
 * @file benchmarks/repeated_hash.cpp
 * @brief Benchmark for repeating a hash function a certain number of times
 */

#include "bench_common.hpp"

volatile uint64_t result;
#define LOOP_HASH(amount)                                                                                                                  \
	BENCHMARK_FN(hash_##amount, {                                                                                                          \
		uint64_t hash = 0;                                                                                                                 \
		for (uint64_t i = 0; i < (amount); i++) {                                                                                          \
			hash = (((hash * 31) + i) >> 2) ^ ((hash << 5) || (hash >> 3));                                                                \
		}                                                                                                                                  \
		result = hash;                                                                                                                     \
	})

LOOP_HASH(1)
LOOP_HASH(2)
LOOP_HASH(4)
LOOP_HASH(8)
LOOP_HASH(16)
LOOP_HASH(32)
LOOP_HASH(64)
LOOP_HASH(128)
LOOP_HASH(256)
LOOP_HASH(512)
LOOP_HASH(1024)
LOOP_HASH(2048)
LOOP_HASH(4096)
LOOP_HASH(8192)
LOOP_HASH(16384)
LOOP_HASH(32768)
LOOP_HASH(65536)
LOOP_HASH(131072)
LOOP_HASH(262144)
LOOP_HASH(524288)
LOOP_HASH(1048576)
LOOP_HASH(2097152)
LOOP_HASH(4194304)
LOOP_HASH(8388608)
LOOP_HASH(16777216)

/**
 * @brief Run the benchmarks
 */
int main() {
	std::ostringstream oss;
	auto result = ankerl::nanobench::Bench()
					  .minEpochIterations(1)
					  .performanceCounters(true)
					  .output(&oss)
					  .RUN(hash_1)
					  .RUN(hash_2)
					  .RUN(hash_4)
					  .RUN(hash_8)
					  .RUN(hash_16)
					  .RUN(hash_32)
					  .RUN(hash_64)
					  .RUN(hash_128)
					  .RUN(hash_256)
					  .RUN(hash_512)
					  .RUN(hash_1024)
					  .RUN(hash_2048)
					  .RUN(hash_4096)
					  .RUN(hash_8192)
					  .RUN(hash_16384)
					  .RUN(hash_32768)
					  .RUN(hash_65536)
					  .RUN(hash_131072)
					  .RUN(hash_262144)
					  .RUN(hash_524288)
					  .RUN(hash_1048576)
					  .RUN(hash_2097152)
					  .RUN(hash_4194304)
					  .RUN(hash_8388608)
					  .RUN(hash_16777216)
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
