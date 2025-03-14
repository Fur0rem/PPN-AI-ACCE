/**
 * @file benchmarks/poisson.cpp
 * @brief Benchmark for algorithms related to the Poisson equation
 */

#include "bench_common.hpp"

#define DECLARE_MATRIX(size)                                                                                                               \
	constexpr double m##size##_a[size][size] = {0};                                                                                        \
	constexpr double m##size##_b[size][size] = {0};                                                                                        \
	double m##size##_c[size][size] = {0};

constexpr int N = 4096;

constexpr int NB_COLS_AB = N;
constexpr int NB_ROWS_AB = 3; // Tri-diagonal matrix
constexpr int PAD_AB = 2;
constexpr int NB_ROWS_TOT_AB = NB_ROWS_AB + PAD_AB;
double ab[NB_COLS_AB * NB_ROWS_TOT_AB] = {0};

BENCHMARK_FN(set_gb_operator_col_major_poisson1d, {
	for (int i = 0; i < NB_COLS_AB * NB_ROWS_TOT_AB; i += NB_ROWS_TOT_AB) {
		// Fill the free space
		for (int j = 0; j < PAD_AB; j++) {
			ab[i + j] = 0;
		}

		// Each line has -1, 2, -1
		ab[i + PAD_AB] = -1;
		ab[i + PAD_AB + 1] = 2;
		ab[i + PAD_AB + 2] = -1;
	}

	// Make the final elem 0
	ab[NB_COLS_AB * NB_ROWS_TOT_AB - 1] = 0;
	// Make the first elem of the first non kv line 0
	ab[PAD_AB] = 0;
})

constexpr int NB_COLS_ID = N;
constexpr int NB_ROWS_ID = 1; // Diagonal matrix
constexpr int PAD_ID = 2;
constexpr int NB_ROWS_TOT_ID = NB_ROWS_ID + PAD_ID;
double id[NB_COLS_ID * NB_ROWS_TOT_ID] = {0};

BENCHMARK_FN(set_gb_operator_col_major_poisson1d_id, {
	// Fill everything with 0's
	for (int i = 0; i < NB_COLS_ID * NB_ROWS_TOT_ID; i++) {
		id[i] = 0;
	}

	// Fill the first last line with 1's (the diagonal)
	for (int i = 0; i < NB_COLS_ID; i++) {
		int idx = i * NB_ROWS_TOT_ID + PAD_ID;
		id[idx] = 1;
	}
})

double rhs[N] = {0};
double ex_sol[N] = {0};
double x[N] = {0};

constexpr double T0 = 50.0;
constexpr double T1 = 100.0;

BENCHMARK_FN(set_dense_rhs_dbc_1d, {
	// Set the Dirichlet boundary condition at the start
	rhs[0] = T0;

	// Set the rest to 0
	for (int i = 1; i < N - 1; i++) {
		rhs[i] = 0.0;
	}

	// Set the Dirichlet boundary condition at the end
	rhs[N - 1] = T1;
})

BENCHMARK_FN(set_analytical_solution_dbc_1d, {
	for (int i = 0; i < N; i++) {
		ex_sol[i] = T0 + x[i] * (T1 - T0);
	}
})

BENCHMARK_FN(set_grid_points_1d, {
	// Uniform grid
	constexpr double step = 1.0 / (1.0 * (N + 1));
	for (int i = 0; i < N; i++) {
		x[i] = (i + 1) * step;
	}
})

double x_exact[N] = {0};
double x_approx[N] = {0};
volatile double result = 0;
BENCHMARK_FN(relative_forward_error, {
	double norm_x = 0;
	for (int i = 0; i < N; i++) {
		norm_x += x_exact[i] * x_exact[i];
	}
	norm_x = sqrt(norm_x);

	for (int i = 0; i < N; i++) {
		x_approx[i] = x_exact[i] - x_approx[i];
	}

	double norm_res = 0;
	for (int i = 0; i < N; i++) {
		norm_res += x_approx[i] * x_approx[i];
	}
	norm_res = sqrt(norm_res);

	result = norm_res / norm_x;
})

#define IDX_AB_COL(i, j) (((j) + 1) * (NB_ROWS_TOT_AB - 1) + (i)-1)

int ipiv[N] = {0};
BENCHMARK_FN(dgbtrftridiag, {
	// Gaussian elimination with partial pivoting
	for (int k = 0; k < N - 1; k++) {
		// Find the biggest pivot
		// We optimise the search by only looking at the next row since it's a tridiagonal matrix
		// and looking further would only lead to 0's
		int pivot_idx;
		double pivot_value;
		if (fabs(ab[IDX_AB_COL(k, k)]) > fabs(ab[IDX_AB_COL(k + 1, k)])) {
			pivot_idx = k;
			pivot_value = ab[IDX_AB_COL(k, k)];
		}
		else {
			pivot_idx = k + 1;
			pivot_value = ab[IDX_AB_COL(k + 1, k)];
		}

		// Swap if necessary
		if (pivot_idx != k) {
			// Swap rows k and pivot_idx
			for (int j = 0; j < NB_ROWS_TOT_AB; j++) {
				double tmp = ab[IDX_AB_COL(pivot_idx, j)];
				ab[IDX_AB_COL(pivot_idx, j)] = ab[IDX_AB_COL(k, j)];
				ab[IDX_AB_COL(k, j)] = tmp;
			}
			// Mark the pivot
			ipiv[k] = pivot_idx;
		}
		else {
			// No row swap
			ipiv[k] = k + 1;
		}

		// Perform the elimination, only the next row is affected since it's a tridiagonal matrix
		if (ab[IDX_AB_COL(k, k)] == 0) {
			// The matrix is singular
			continue;
		}
		ab[IDX_AB_COL(k + 1, k)] /= ab[IDX_AB_COL(k, k)];
		ab[IDX_AB_COL(k + 1, k + 1)] -= ab[IDX_AB_COL(k + 1, k)] * ab[IDX_AB_COL(k, k + 1)];
	}
	// Mark the last pivot
	ipiv[N - 1] = N;
})

constexpr double ALPHA = 1.0;
constexpr double BETA = 3.0;
constexpr int INCX = 2;
constexpr int INCY = 2;

BENCHMARK_FN(dgbmv_no_transposed, {
	for (int i = 0; i < N; i += INCY) {
		double sum = 0;
		for (int j = 0; j < NB_ROWS_TOT_AB; j += INCX) {
			sum += ab[IDX_AB_COL(j, i)] * x[j];
		}
		x[i] = ALPHA * sum + BETA * x[i];
	}
})

BENCHMARK_FN(dgbmv_transposed, {
	double result[N] = {0};
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < NB_ROWS_TOT_AB; j++) {
			result[i] += ab[IDX_AB_COL(j, i)] * x[j];
		}
	}
	for (int i = 0; i < N; i++) {
		x[i] = ALPHA * result[i] + BETA * x[i];
	}
})

double eigval[N] = {0};
BENCHMARK_FN(eig_poisson_1d, {
	double h = 1.0 / (N + 1);
	// λ^k = 4 sin²(kπh / 2)
	for (int k = 1; k <= N; k++) {
		double sin_theta = sin(k * M_PI * h / 2.0);
		eigval[k - 1] = 4.0 * sin_theta * sin_theta;
	}
})

double res[N];
double b[N];

BENCHMARK_FN(residual, {
	for (int i = 0; i < N; i++) {
		res[i] = b[i];
		// Difference between actual result and computed result
		for (int j = 0; j < N; j++) {
			res[i] -= ab[IDX_AB_COL(j, i)] * x[j];
		}
	}
})

void init_arrays() {
	for (int i = 0; i < N; i++) {
		b[i] = 5.0;
	}
	___set_gb_operator_col_major_poisson1d_start_PPN_LABEL();
	___set_gb_operator_col_major_poisson1d_id_start_PPN_LABEL();
	___set_dense_rhs_dbc_1d_start_PPN_LABEL();
	___set_analytical_solution_dbc_1d_start_PPN_LABEL();
	___set_grid_points_1d_start_PPN_LABEL();
}

/**
 * @brief Run the benchmarks for poisson equation
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(50, oss)
					  .RUN(set_gb_operator_col_major_poisson1d)
					  .RUN(set_gb_operator_col_major_poisson1d_id)
					  .RUN(set_dense_rhs_dbc_1d)
					  .RUN(set_analytical_solution_dbc_1d)
					  .RUN(set_grid_points_1d)
					  .RUN(dgbtrftridiag)
					  .RUN(relative_forward_error)
					  .RUN(dgbmv_no_transposed)
					  .RUN(dgbmv_transposed)
					  .RUN(eig_poisson_1d)
					  .RUN(residual)
					  .results();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
