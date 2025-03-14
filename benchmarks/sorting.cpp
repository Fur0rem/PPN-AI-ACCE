/**
 * @file benchmarks/sorting.cpp
 * @brief Benchmark for sorting algorithms
 */

#include "bench_common.hpp"
#include <ctime>

constexpr int N = 2048; ///< Size of the arrays
double arr[N];			///< Array of double precision floating point numbers to sort

BENCHMARK_FN(init_arrays, {
	srand(time(nullptr));
	for (int i = 0; i < N; i++) {
		arr[i] = (double)rand() / RAND_MAX;
	}
})

BENCHMARK_FN(bubble_sort, {
	for (int i = 0; i < N - 1; i++) {
		for (int j = 0; j < N - i - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				double temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
})

BENCHMARK_FN(selection_sort, {
	for (int i = 0; i < N - 1; i++) {
		int min = i;
		for (int j = i + 1; j < N; j++) {
			if (arr[j] < arr[min]) {
				min = j;
			}
		}
		double temp = arr[min];
		arr[min] = arr[i];
		arr[i] = temp;
	}
})

BENCHMARK_FN(insertion_sort, {
	for (int i = 1; i < N; i++) {
		double key = arr[i];
		int j = i - 1;
		while (j >= 0 && arr[j] > key) {
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = key;
	}
})

OPTIMISE_SIZE
void merge_sort_start_PPN_LABEL(double arr[], int l, int r) {
	if (l < r) {
		int m = l + (r - l) / 2;

		merge_sort_start_PPN_LABEL(arr, l, m);
		merge_sort_start_PPN_LABEL(arr, m + 1, r);

		int i, j, k;
		int n1 = m - l + 1;
		int n2 = r - m;

		double left[n1], right[n2];

		for (i = 0; i < n1; i++) {
			left[i] = arr[l + i];
		}
		for (j = 0; j < n2; j++) {
			right[j] = arr[m + 1 + j];
		}

		i = 0;
		j = 0;
		k = l;
		while (i < n1 && j < n2) {
			if (left[i] <= right[j]) {
				arr[k] = left[i];
				i++;
			}
			else {
				arr[k] = right[j];
				j++;
			}
			k++;
		}

		while (i < n1) {
			arr[k] = left[i];
			i++;
			k++;
		}

		while (j < n2) {
			arr[k] = right[j];
			j++;
			k++;
		}
	}
}

void quick_sort_start_PPN_LABEL(double arr[], int low, int high) {
	if (low < high) {
		int pi = low - 1;
		double pivot = arr[high];
		for (int i = low; i < high; i++) {
			if (arr[i] < pivot) {
				pi++;
				double temp = arr[pi];
				arr[pi] = arr[i];
				arr[i] = temp;
			}
		}
		double temp = arr[pi + 1];
		arr[pi + 1] = arr[high];
		arr[high] = temp;

		quick_sort_start_PPN_LABEL(arr, low, pi);
		quick_sort_start_PPN_LABEL(arr, pi + 2, high);
	}
}

/**
 * @brief Run the benchmarks for dummy
 */
int main() {
	std::ostringstream oss;
	auto result = BENCHMARK_INIT(100, oss).RUN(init_arrays).RUN(bubble_sort).doNotOptimizeAway(arr).results();

	___init_arrays_start_PPN_LABEL();
	concat_results(result, BENCHMARK_INIT(100, oss).RUN(selection_sort).doNotOptimizeAway(arr).results());

	___init_arrays_start_PPN_LABEL();
	concat_results(result, BENCHMARK_INIT(100, oss).RUN(insertion_sort).doNotOptimizeAway(arr).results());

	___init_arrays_start_PPN_LABEL();
	concat_results(result,
				   BENCHMARK_INIT(100, oss)
					   .run("merge_sort_start_PPN_LABEL",
							[&]() {
								merge_sort_start_PPN_LABEL(arr, 0, N - 1);
							})
					   .doNotOptimizeAway(arr)
					   .results());

	___init_arrays_start_PPN_LABEL();
	concat_results(result,
				   BENCHMARK_INIT(100, oss)
					   .run("quick_sort_start_PPN_LABEL",
							[&]() {
								quick_sort_start_PPN_LABEL(arr, 0, N - 1);
							})
					   .doNotOptimizeAway(arr)
					   .results());

	// std::cout << oss.str();

	for (auto const& res : result) {
		auto measure = res.fromString("cpucycles");
		auto name = res.config().mBenchmarkName;
		std::cout << name << " : ";
		std::cout << res.median(measure) << " cycles\n";
	}

	return 0;
}
