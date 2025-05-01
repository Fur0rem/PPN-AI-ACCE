results = {
	"Early Stopping": {
		1: {
			"min": 396.609,
			"max": 532.555,
			"med": 464.582
		},
		2: {
			"min": 393.4,
			"max": 423.906,
			"med": 408.653
		},
		3: {
			"min": 239.003,
			"max": 267.813,
			"med": 253.408
		},
		4: {
			"min": 137.844,
			"max": 232.65,
			"med": 185.247
		},
		5: {
			"min": 134.057,
			"max": 228.771,
			"med": 174.91
		},
		6: {
			"min": 167.271,
			"max": 248.686,
			"med": 191.478
		},
		7: {
			"min": 131.453,
			"max": 187.754,
			"med": 165.604
		},
		8: {
			"min": 155.909,
			"max": 160.824,
			"med": 158.366
		}
	},
	"Without Early Stopping": {
		1: {
			"min": 469.191,
			"max": 533.625,
			"med": 501.408
		},
		2: {
			"min": 372.469,
			"max": 448.41,
			"med": 435.439
		},
		3: {
			"min": 280.888,
			"max": 303.601,
			"med": 292.245
		},
		4: {
			"min": 288.274,
			"max": 304.059,
			"med": 296.166
		},
		5: {
			"min": 266.034,
			"max": 292.872,
			"med": 280.453
		},
		6: {
			"min": 206.971,
			"max": 291.024,
			"med": 248.997
		},
		7: {
			"min": 203.11,
			"max": 282.14,
			"med": 259.99
		},
		8: {
			"min": 215.889,
			"max": 256.241,
			"med": 236.065
		}
	}
}

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# Plot the results for the strong scaling study for both early stopping and without early stopping
def plot_strong_scaling(results):
	# Create a DataFrame from the results
	data = {
		"Threads": [],
		"Time (s)": [],
		"Type": [],
		"yerr": []
	}

	for key, value in results.items():
		for threads, times in value.items():
			data["Threads"].append(threads)
			data["Time (s)"].append(times["med"])
			data["yerr"].append((times["max"] - times["min"]) / 2)
			data["Type"].append(key)

	df = pd.DataFrame(data)

	# Plot the results
	plt.figure(figsize=(10, 6))

	# Plot the early stopping results
	early_stopping = df[df["Type"] == "Early Stopping"]
	plt.plot(early_stopping["Threads"], early_stopping["Time (s)"], marker='o', label="Early Stopping", color="blue")
	# Plot the without early stopping results
	without_early_stopping = df[df["Type"] == "Without Early Stopping"]
	plt.plot(without_early_stopping["Threads"], without_early_stopping["Time (s)"], marker='o', label="Without Early Stopping", color="orange")

	# Plot the ideal scaling line for both cases
	ideal_scaling_without = [results["Without Early Stopping"][1]["med"] / t for t in df["Threads"].unique()]
	ideal_scaling_with = [results["Early Stopping"][1]["med"] / t for t in df["Threads"].unique()]
	plt.plot(df["Threads"].unique(), ideal_scaling_with, linestyle="dotted", label="Ideal Scaling With Early Stopping", color="blue")
	plt.plot(df["Threads"].unique(), ideal_scaling_without, linestyle="dotted", label="Ideal Scaling Without Early Stopping", color="orange")
	
	# Plot error bars for early stopping
	plt.errorbar(early_stopping["Threads"], early_stopping["Time (s)"], yerr=early_stopping["yerr"], fmt='o', color='blue', capsize=5)
	# Plot error bars for without early stopping
	plt.errorbar(without_early_stopping["Threads"], without_early_stopping["Time (s)"], yerr=without_early_stopping["yerr"], fmt='o', color='orange', capsize=5)

	
	# Add labels and title
	plt.xticks(df["Threads"].unique())
	plt.legend(shadow=True, loc="upper right")
	# Make the ticks bigger and labels bigger
	plt.tick_params(axis='both', which='major', labelsize=14)
	plt.xlabel("Number of Threads", fontsize=16)
	plt.ylabel("Time (s)", fontsize=16)
	plt.grid(True)
	plt.show()

# Call the function to plot the strong scaling results
plot_strong_scaling(results)
