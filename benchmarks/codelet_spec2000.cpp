/**
 * @file benchmarks/codelet_spec2000.cpp
 * @brief Benchmark extracted from the codelet of the SPEC2000 benchmark
 */

#include "bench_common.hpp"

constexpr int NX = 184;
constexpr int J = 92;
int chanx_occ[NX + 1][J + 1];
volatile float result;
volatile int result_int;

BENCHMARK_FN(vpr_stats_routing_line88, {
	float av_occ = 0.000000;
	int max_occ = -1;
	for (int i = 1; i <= NX; i += 1) {
		max_occ = (chanx_occ[i][J] > max_occ ? chanx_occ[i][J] : max_occ);
		av_occ += chanx_occ[i][J];
	}
	av_occ = av_occ / NX;
	result = av_occ;
	result_int = max_occ;
})

constexpr int NUMF1S = 10000;
constexpr int START = 1;
constexpr int NUMF2S = 3;
volatile double tds[NUMF1S][NUMF2S] = {0.0};

BENCHMARK_FN(scanner_c_init_td_line819_loop, {
	for (int i = 0; i <= NUMF1S - 1; i += 1) {
		for (int j = START; j <= NUMF2S - 1; j += 1) {
			tds[i][j] = 0.0;
		}
	}
})

constexpr int ARCHNODES = 30169;
volatile double disp[2][ARCHNODES][3] = {0.0};

BENCHMARK_FN(quake_c_main_line452_loop, {
	for (int i = 0; i <= ARCHNODES - 1; i += 1) {
		for (int j = 0; j <= 2; j += 1) {
			disp[1][i][j] = 0.0;
		}
	}
})

typedef struct {
	double dt;
	double duration;
	double t0;
} Excitation;

constexpr double EXC_DT = 0.002400;
constexpr Excitation EXC = {EXC_DT, 0.0, 0.0};

BENCHMARK_FN(quake_c_main_line461_loop, {
	for (int i = 0; i <= ARCHNODES - 1; i += 1) {
		for (int j = 0; j <= 2; j += 1) {
			disp[1][i][j] *= -EXC.dt * EXC.dt;
		}
	}
})

volatile double m[ARCHNODES][3] = {0.0};
volatile double c[ARCHNODES][3] = {0.0};

BENCHMARK_FN(quake_c_main_line473_loop, {
	for (int i = 0; i <= ARCHNODES - 1; i += 1) {
		for (int j = 0; j <= 2; j += 1) {
			disp[1][i][j] = disp[1][i][j] / (m[i][j] + EXC.dt / 2.0 * c[i][j]);
		}
	}
})

constexpr int ARCHMATRIXLEN = 220546;
volatile double*** k;

BENCHMARK_FN(quake_c_mem_init_line1508_loop, {
	for (int i = 0; i <= ARCHMATRIXLEN - 1; i += 1) {
		for (int j = 0; j <= 2; j += 1) {
			for (int kk = 0; kk <= 2; kk += 1) {
				k[i][j][kk] = 0.0;
			}
		}
	}
})

void init_arrays() {
	k = new volatile double**[ARCHMATRIXLEN];
	for (int i = 0; i < ARCHMATRIXLEN; i++) {
		k[i] = new volatile double*[3];
		for (int j = 0; j < 3; j++) {
			k[i][j] = new volatile double[3];
		}
	}

	for (int i = 0; i < NX + 1; i++) {
		for (int j = 0; j < J + 1; j++) {
			chanx_occ[i][j] = rand() % 100;
		}
	}

	for (int i = 0; i < NUMF1S; i++) {
		for (int j = 0; j < NUMF2S; j++) {
			tds[i][j] = rand() % 100;
		}
	}

	for (int i = 0; i < ARCHNODES; i++) {
		for (int j = 0; j < 3; j++) {
			disp[1][i][j] = rand() % 100;
		}
	}

	for (int i = 0; i < ARCHNODES; i++) {
		for (int j = 0; j < 3; j++) {
			m[i][j] = rand() % 100;
		}
	}

	for (int i = 0; i < ARCHNODES; i++) {
		for (int j = 0; j < 3; j++) {
			c[i][j] = rand() % 100;
		}
	}

	for (int i = 0; i < ARCHMATRIXLEN; i++) {
		for (int j = 0; j < 3; j++) {
			for (int kk = 0; kk < 3; kk++) {
				k[i][j][kk] = rand() % 100;
			}
		}
	}
}

/**
 * @brief Run the benchmarks for stencil_c_main_line259_loop_c_0_c
 */
int main() {
	std::ostringstream oss;
	init_arrays();
	auto result = BENCHMARK_INIT(10, oss)
					  .RUN(vpr_stats_routing_line88)
					  .RUN(scanner_c_init_td_line819_loop)
					  .RUN(quake_c_main_line452_loop)
					  .RUN(quake_c_main_line461_loop)
					  .RUN(quake_c_main_line473_loop)
					  .RUN(quake_c_mem_init_line1508_loop)
					  .doNotOptimizeAway(k)
					  .doNotOptimizeAway(disp)
					  .doNotOptimizeAway(c)
					  .doNotOptimizeAway(m)
					  .doNotOptimizeAway(chanx_occ)
					  .doNotOptimizeAway(tds)
					  .doNotOptimizeAway(EXC)
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
