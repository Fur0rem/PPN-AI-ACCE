# Naive Multiplication, Min: 3.03209s, Max: 3.03602s, Med: 3.03258s
# Sparse Multiplication, Min: 0.811475s, Max: 0.812206s, Med: 0.811583s
# Blocked-8 Sparse Multiplication, Min: 1.34089s, Max: 1.35019s, Med: 1.34465s
# Blocked-16 Sparse Multiplication, Min: 1.13534s, Max: 1.13805s, Med: 1.1362s
# Blocked-32 Sparse Multiplication, Min: 1.06327s, Max: 1.06684s, Med: 1.06522s
# Eigen Sparse Multiplication, Min: 1.2649s, Max: 1.44142s, Med: 1.38434s
# Eigen Multiplication, Min: 0.0171376s, Max: 0.0176137s, Med: 0.0175167s

results = {
	"Naive": {
		"Min": 3.03209,
		"Max": 3.03602,
		"Median": 3.03258,
	},
	"Sparse": {
		"Min": 0.811475,
		"Max": 0.812206,
		"Median": 0.811583,
	},
	"Blocked-8 Sparse": {
		"Min": 1.34089,
		"Max": 1.35019,
		"Median": 1.34465,
	},
	"Blocked-16 Sparse": {
		"Min": 1.13534,
		"Max": 1.13805,
		"Median": 1.1362,
	},
	"Blocked-32 Sparse": {
		"Min": 1.06327,
		"Max": 1.06684,
		"Median": 1.06522,
	},
	"Eigen Sparse": {
		"Min": 1.2649,
		"Max": 1.44142,
		"Median": 1.38434,
	},
	"Eigen": {
		"Min": 0.0171376,
		"Max": 0.0176137,
		"Median": 0.0175167
	}
}

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def plot_sparse_multiplication_benchmarks(results):
	# Create a DataFrame from the results
	data = {
		"Method": [],
		"Time (ms)": [],
	}
	for method, times in results.items():
		data["Method"].append(method)
		data["Time (ms)"].append(times["Median"])

	df = pd.DataFrame(data)

	# Plot the results
	plt.figure(figsize=(10, 6))
	plt.bar(df["Method"], df["Time (ms)"], color='blue')
	# Add some text above the bars to show median time and error
	for i, row in df.iterrows():
		plt.text(i, row["Time (ms)"], f"{row['Time (ms)'] * 1000:.1f} ± {((results[row['Method']]['Max'] - results[row['Method']]['Min'])/2) * 1000:.2f}", ha='center', va='bottom')
	plt.xticks(rotation=45)
	plt.ylabel("Time (ms)")
	plt.xlabel("Method")
	plt.title("Sparse Multiplication Benchmarks")
	plt.grid(axis='y', alpha = 0.5)
	plt.tight_layout()
	plt.show()
# Call the function to plot the sparse multiplication benchmarks
plot_sparse_multiplication_benchmarks(results)
