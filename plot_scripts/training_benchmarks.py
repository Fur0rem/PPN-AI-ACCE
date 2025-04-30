import matplotlib.pyplot as plt
import numpy as np

# Train, Min: 25.659s, Max: 25.6969s, Med: 25.6779s
# ----------------------------

#  Performance counter stats for './build/benchmark_training':

#     11 926 657 067      cache-references:u                                                      (93,53%)
#        698 897 930      cache-misses:u                   #    5,86% of all cache refs           (93,54%)

#       51,442786478 seconds time elapsed

#      251,841462000 seconds user
#       10,344400000 seconds sys

# Train, Min: 21.6034s, Max: 21.8576s, Med: 21.7305s
# ----------------------------

#  Performance counter stats for './build/benchmark_training':

#      6 486 469 596      cache-references:u                                                      (94,08%)
#        392 340 524      cache-misses:u                   #    6,05% of all cache refs           (94,08%)

#       43,536131111 seconds time elapsed

#      232,703776000 seconds user
#        9,975374000 seconds sys


# In seconds
nb_epochs = 20
results = {
	# Train, Min: 25.2005s, Max: 27.0017s, Med: 26.1011s
	"Baseline": {
		"Min": 25.9005,
		"Max": 27.0017,
		"Median": 26.2011,
	},
	# Train, Min: 21.2048s, Max: 22.0654s, Med: 21.6351s
	"Swapped Loops": {
		"Min": 21.2048,
		"Max": 22.0654,
		"Median": 21.6351,
	},
}

# def plot_comparaison(name_ref: str, name_new: str) :
# 	"""
# 	Plot the number of epochs in can do per second using bar plots
# 	"""
# 	fig, ax = plt.subplots(figsize=(6, 6))
# 	# Plot the average training time for each method
# 	labels = [name_ref, name_new]
# 	x = np.arange(len(labels))
# 	width = 0.35  # the width of the bars
# 	avg_times = [results[name_ref]["Average"], results[name_new]["Average"]]
# 	avg_times = [nb_epochs / t for t in avg_times]
# 	rects1 = ax.bar(x, avg_times, width, label='Average')
# 	# Change the color of the bars
# 	rects1[0].set_color('red')
# 	rects1[1].set_color('green')

# 	# Add labels under the bars
# 	ax.set_xticks(x)
# 	ax.set_xticklabels(labels)

# 	# Put the values on top of the bars
# 	def autolabel(rects):
# 		"""Attach a text label above each bar in *rects*, displaying its height."""
# 		for rect in rects:
# 			height = rect.get_height()
# 			ax.annotate(f"{height:.2f} epochs/s",
# 						xy=(rect.get_x() + rect.get_width() / 2, height),
# 						xytext=(0, 3),  # 3 points vertical offset
# 						textcoords="offset points",
# 						ha='center', va='bottom', size=12)
# 	autolabel(rects1)

# 	# Make the ticks bigger
# 	ax.tick_params(axis='both', which='major', labelsize=14)
# 	# Make the labels bigger
# 	ax.set_xlabel("Optimisation method", fontsize=16)
# 	ax.set_ylabel("Epochs per second", fontsize=16)
	
	
# 	plt.tight_layout()
# 	plt.show()


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
	# Change the color of the bars
	rects1[0].set_color('red')
	rects1[1].set_color('green')

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
	

plot_comparaison(["Baseline", "Sparse Computation", "Sparse Computation (no cache)"])