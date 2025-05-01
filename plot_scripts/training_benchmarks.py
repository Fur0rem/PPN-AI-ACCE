import matplotlib.pyplot as plt
import numpy as np

# In seconds
nb_epochs = 20
results = {
	# Train, Min: 27.1807s, Max: 27.3425s, Med: 27.2616s
	"Baseline": {
		"Min": 27.1807,
		"Max": 27.3425,
		"Median": 27.2616,
	},
	# Train, Min: 22.7219s, Max: 22.7406s, Med: 22.7312s
	"Swapped Loops": {
		"Min": 22.7219,
		"Max": 22.7406,
		"Median": 22.7312,
	},
	# 4 : Train, Min: 20.2409s, Max: 20.3506s, Med: 20.2957s
	"Parallel Matrix Multiplication": {
		"Min": 20.2409,
		"Max": 20.3506,
		"Median": 20.2957,
	},
	# Train, Min: 17.932s, Max: 17.9976s, Med: 17.9648s
	"Parallel Optimisers": {
		"Min": 17.932,
		"Max": 17.9976,
		"Median": 17.9648,
	},
	# Train, Min: 17.5122s, Max: 17.6001s, Med: 17.5561s
	"Sparse Computation": {
		"Min": 17.5122,
		"Max": 17.6001,
		"Median": 17.5561,
	},
	# Train, Min: 16.8776s, Max: 16.8805s, Med: 16.879s
	"Mathemetical Approximations": {
		"Min": 16.8776,
		"Max": 16.8805,
		"Median": 16.879,
	},
}

def plot_before_after(baseline: str, optimised: str):
	"""
	Plot the number of epochs it can do per second using bar plots with error as text.
	"""
	fig, ax = plt.subplots(figsize=(8, 8))
	# Plot the median training time for each method
	labels = [baseline, optimised]
	x = np.arange(len(labels))
	width = 0.4  # the width of the bars
	avg_times = [results[name]["Median"] for name in labels]
	avg_times = [nb_epochs / t for t in avg_times]

	# Calculate errors
	errors = [
		[
			nb_epochs / results[name]["Median"] - nb_epochs / results[name]["Max"],  # Lower error
			nb_epochs / results[name]["Min"] - nb_epochs / results[name]["Median"],  # Upper error
		]
		for name in labels
	]

	# Plot bars
	rects1 = ax.bar(x, avg_times, width, label='Median')
	# Change the color of the bars
	rects1[0].set_color('red')
	rects1[1].set_color('green')

	# Add labels under the bars
	ax.set_xticks(x)
	ax.set_xticklabels(["Before", "After"])

	# Put the values on top of the bars with error
	def autolabel(rects, errors):
		"""Attach a text label above each bar in *rects*, displaying its height ± error."""
		for rect, error in zip(rects, errors):
			height = rect.get_height()
			lower_error = abs(error[0])  # Lower error
			upper_error = abs(error[1])  # Upper error
			ax.annotate(f"{height:.4f} ± {max(lower_error, upper_error):.4f} epochs/s",
						xy=(rect.get_x() + rect.get_width() / 2, height),
						xytext=(0, 3),  # 3 points vertical offset
						textcoords="offset points",
						ha='center', va='bottom', size=12)

	autolabel(rects1, errors)

	# Make the ticks bigger
	ax.tick_params(axis='both', which='major', labelsize=12)
	# Make the labels bigger
	ax.set_xlabel(f"Before and after {optimised} optimisation", fontsize=18)
	ax.set_ylabel("Epochs per second", fontsize=16)
	
	plt.tight_layout()
	plt.show()

def plot_comparaison(all_names: list[str]):
	"""
	Plot the number of epochs in can do per second using bar plots
	"""
	fig, ax = plt.subplots(figsize=(8, 8))
	# Plot the median training time for each method
	labels = all_names
	x = np.arange(len(labels))
	width = 0.4  # the width of the bars
	avg_times = [results[name]["Median"] for name in all_names]
	avg_times = [nb_epochs / t for t in avg_times]
	rects1 = ax.bar(x, avg_times, width, label='Median')

	# Add labels under the bars
	ax.set_xticks(x)
	ax.set_xticklabels(labels)

	# Put the values on top of the bars
	def autolabel(rects):
		"""Attach a text label above each bar in *rects*, displaying its height."""
		for rect in rects:
			height = rect.get_height()
			ax.annotate(f"{height:.2f} epochs/s",
						xy=(rect.get_x() + rect.get_width() / 2, height),
						xytext=(0, 3),  # 3 points vertical offset
						textcoords="offset points",
						ha='center', va='bottom', size=12)
	autolabel(rects1)

	# Make the ticks bigger
	ax.tick_params(axis='both', which='major', labelsize=12)
	# Make the labels bigger
	ax.set_xlabel("Optimisation method", fontsize=18)
	ax.set_ylabel("Epochs per second", fontsize=16)
	
	plt.tight_layout()
	plt.show()
	

plot_before_after("Baseline", "Swapped Loops")
plot_comparaison(["Baseline", "Swapped Loops", "Parallel Matrix Multiplication", "Parallel Optimisers", "Sparse Computation", "Mathemetical Approximations"])