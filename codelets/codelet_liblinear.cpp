/**
 * @file codelets/liblinear.cpp
 * @brief Benchmark for liblinear
 */

#include "bench_common.hpp"

constexpr int W_SIZE = 2014669;
double* w;
double f = 0.0;

/**
 * @brief Perform the loop for dot product with itself
 */
BENCHMARK_FN(c_l2r_l2_svc_fun_fun_line221, {
	for (int i = 0; i <= W_SIZE - 1; i += 1) {
		f += w[i] * w[i];
	}
})

double* g;

/**
 * @brief Perform the loop for gradient
 */
BENCHMARK_FN(c_l2r_l2_svc_fun_grad_line254, {
	for (int i = 0; i <= W_SIZE - 1; i += 1) {
		g[i] = w[i] + 2 * g[i];
	}
})

double* hs;
double* s;

/**
 * @brief Perform the loop for Hessian vector product
 */
BENCHMARK_FN(c_l2r_l2_svc_fun_hv_line275, {
	for (int i = 0; i <= W_SIZE - 1; i += 1) {
		hs[i] = s[i] + 2 * hs[i];
	}
})

double* xtv;

/**
 * @brief Perform the loop for sub XTv
 */
BENCHMARK_FN(c_l2r_l2_svc_fun_sub_xtv_line323, {
	for (int i = 0; i <= W_SIZE - 1; i += 1) {
		xtv[i] = 0;
	}
})

constexpr int L = 510302;
double alpha[L] = {13.0};

/**
 * @brief Perform the loop for alpha
 */
BENCHMARK_FN(c_solve_l2r_l1l2_svc_line972, {
	for (int i = 0; i <= L - 1; i += 1) {
		alpha[i] = 0;
	}
})

constexpr int N = 2014669;
double* s2;
double* r2;
double* g2;
double* d2;

/**
 * @brief Perform the loop for s, r and d
 */
BENCHMARK_FN(c_tron_trcg_l2r_l2_svc_line417, {
	for (int i = 0; i <= N - 1; i += 1) {
		s2[i] = 0;
		r2[i] = -g2[i];
		d2[i] = r2[i];
	}
})

/**
 * @brief Run the benchmarks for codelet_generate_seq_laplacian_line73
 */
int main() {
	std::ostringstream oss;
	s2 = new double[N];
	r2 = new double[N];
	g2 = new double[N];
	d2 = new double[N];
	for (int i = 0; i < N; i++) {
		s2[i] = 1.0;
		r2[i] = 2.0;
		g2[i] = 5.0;
		d2[i] = 3.0;
	}
	w = new double[W_SIZE];
	g = new double[W_SIZE];
	hs = new double[W_SIZE];
	s = new double[W_SIZE];
	xtv = new double[W_SIZE];
	for (int i = 0; i < W_SIZE; i++) {
		w[i] = 1.0;
		g[i] = 2.0;
		hs[i] = 5.0;
		s[i] = 3.0;
		xtv[i] = 4.0;
	}
	auto result = BENCHMARK_INIT(100, oss)
					  .RUN(c_l2r_l2_svc_fun_fun_line221)
					  .RUN(c_l2r_l2_svc_fun_grad_line254)
					  .RUN(c_l2r_l2_svc_fun_hv_line275)
					  .RUN(c_l2r_l2_svc_fun_sub_xtv_line323)
					  .RUN(c_solve_l2r_l1l2_svc_line972)
					  .RUN(c_tron_trcg_l2r_l2_svc_line417)
					  .doNotOptimizeAway(w)
					  .doNotOptimizeAway(f)
					  .doNotOptimizeAway(g)
					  .doNotOptimizeAway(alpha)
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
