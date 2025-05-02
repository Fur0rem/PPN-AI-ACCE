results_1 = """
Size: 100x100
CPU, Time: 9.92151e-05s
GPU, Time: 0.087804s
----------------------------
Size: 200x200
CPU, Time: 0.000840097s
GPU, Time: 0.0684425s
----------------------------
Size: 300x300
CPU, Time: 0.00308356s
GPU, Time: 0.0711774s
----------------------------
Size: 400x400
CPU, Time: 0.0056019s
GPU, Time: 0.0661268s
----------------------------
Size: 500x500
CPU, Time: 0.00723367s
GPU, Time: 0.074037s
----------------------------
Size: 600x600
CPU, Time: 0.0120807s
GPU, Time: 0.0712694s
----------------------------
Size: 700x700
CPU, Time: 0.028553s
GPU, Time: 0.0883854s
----------------------------
Size: 800x800
CPU, Time: 0.0278405s
GPU, Time: 0.0748981s
----------------------------
Size: 900x900
CPU, Time: 0.039287s
GPU, Time: 0.0868319s
----------------------------
Size: 1000x1000
CPU, Time: 0.0533888s
GPU, Time: 0.0946972s
----------------------------
Size: 1100x1100
CPU, Time: 0.0657206s
GPU, Time: 0.0993596s
----------------------------
Size: 1200x1200
CPU, Time: 0.0827396s
GPU, Time: 0.107641s
----------------------------
Size: 1300x1300
CPU, Time: 0.0992459s
GPU, Time: 0.136183s
----------------------------
Size: 1400x1400
CPU, Time: 0.111982s
GPU, Time: 0.152018s
----------------------------
Size: 1500x1500
CPU, Time: 0.1566s
GPU, Time: 0.145866s
----------------------------
"""

results_2 = """
Size: 100x100
CPU, Time: 0.000695495s
GPU, Time: 0.0941622s
----------------------------
Size: 200x200
CPU, Time: 0.000687841s
GPU, Time: 0.0711969s
----------------------------
Size: 300x300
CPU, Time: 0.00244866s
GPU, Time: 0.0683997s
----------------------------
Size: 400x400
CPU, Time: 0.00465821s
GPU, Time: 0.0637769s
----------------------------
Size: 500x500
CPU, Time: 0.00741799s
GPU, Time: 0.0687785s
----------------------------
Size: 600x600
CPU, Time: 0.0126791s
GPU, Time: 0.0692033s
----------------------------
Size: 700x700
CPU, Time: 0.0198781s
GPU, Time: 0.0817374s
----------------------------
Size: 800x800
CPU, Time: 0.027763s
GPU, Time: 0.0855698s
----------------------------
Size: 900x900
CPU, Time: 0.0399776s
GPU, Time: 0.0849502s
----------------------------
Size: 1000x1000
CPU, Time: 0.053844s
GPU, Time: 0.0916522s
----------------------------
Size: 1100x1100
CPU, Time: 0.0672056s
GPU, Time: 0.100245s
----------------------------
Size: 1200x1200
CPU, Time: 0.0838328s
GPU, Time: 0.108726s
----------------------------
Size: 1300x1300
CPU, Time: 0.106s
GPU, Time: 0.118622s
----------------------------
Size: 1400x1400
CPU, Time: 0.1111898s
GPU, Time: 0.131308s
----------------------------
Size: 1500x1500
CPU, Time: 0.158903s
GPU, Time: 0.15123s
----------------------------
"""

results_3 = """
Size: 100x100
CPU, Time: 9.0446e-05s
GPU, Time: 0.0813629s
----------------------------
Size: 200x200
CPU, Time: 0.000575976s
GPU, Time: 0.0578662s
----------------------------
Size: 300x300
CPU, Time: 0.0027203s
GPU, Time: 0.0577483s
----------------------------
Size: 400x400
CPU, Time: 0.00613172s
GPU, Time: 0.0620721s
----------------------------
Size: 500x500
CPU, Time: 0.008337s
GPU, Time: 0.0660472s
----------------------------
Size: 600x600
CPU, Time: 0.0119052s
GPU, Time: 0.0771497s
----------------------------
Size: 700x700
CPU, Time: 0.0187886s
GPU, Time: 0.0697436s
----------------------------
Size: 800x800
CPU, Time: 0.0272497s
GPU, Time: 0.0766045s
----------------------------
Size: 900x900
CPU, Time: 0.0389141s
GPU, Time: 0.0901894s
----------------------------
Size: 1000x1000
CPU, Time: 0.0495045s
GPU, Time: 0.0989255s
----------------------------
Size: 1100x1100
CPU, Time: 0.0713868s
GPU, Time: 0.101663s
----------------------------
Size: 1200x1200
CPU, Time: 0.0839666s
GPU, Time: 0.112148s
----------------------------
Size: 1300x1300
CPU, Time: 0.09001s
GPU, Time: 0.119184s
----------------------------
Size: 1400x1400
CPU, Time: 0.121853s
GPU, Time: 0.131966s
----------------------------
Size: 1500x1500
CPU, Time: 0.159241s
GPU, Time: 0.14643s
----------------------------
"""

import matplotlib.pyplot as plt
import numpy as np
import re

# Parse results
def parse_results(results):
	sizes = []
	cpu_times = []
	gpu_times = []
	for line in results.strip().split("\n"):
		if "Size" in line:
			size = int(re.search(r"Size: (\d+)x\d+", line).group(1))
			sizes.append(size)
		elif "CPU" in line:
			cpu_time = float(re.search(r"CPU, Time: ([\deE\.\-]+)s", line).group(1))
			cpu_times.append(cpu_time)
		elif "GPU" in line:
			gpu_time = float(re.search(r"GPU, Time: ([\deE\.\-]+)s", line).group(1))
			gpu_times.append(gpu_time)
	return sizes, cpu_times, gpu_times

# Combine results
def combine_results(results_list):
	sizes = None
	cpu_times_list = []
	gpu_times_list = []
	for results in results_list:
		parsed_sizes, cpu_times, gpu_times = parse_results(results)
		if sizes is None:
			sizes = parsed_sizes
		cpu_times_list.append(cpu_times)
		gpu_times_list.append(gpu_times)
	return sizes, np.array(cpu_times_list), np.array(gpu_times_list)

# Calculate mean and standard deviation
def calculate_stats(data):
	mean = np.mean(data, axis=0)
	stddev = np.std(data, axis=0)
	return mean, stddev

# Results
results_list = [results_1, results_2, results_3]
sizes, cpu_times_list, gpu_times_list = combine_results(results_list)

# Calculate stats
cpu_mean, cpu_stddev = calculate_stats(cpu_times_list)
gpu_mean, gpu_stddev = calculate_stats(gpu_times_list)

# Plot
plt.figure(figsize=(10, 6))
plt.errorbar(sizes, cpu_mean, yerr=cpu_stddev, fmt="-o", label="CPU", capsize=5, color="blue")
plt.errorbar(sizes, gpu_mean, yerr=gpu_stddev, fmt="-o", label="GPU", capsize=5, color="green")

# Labels and legend
plt.xlabel("Matrix Size (NxN)", fontsize=14)
plt.ylabel("Time (s)", fontsize=14)
# plt.title("CPU vs GPU Performance Comparison", fontsize=16)
plt.legend(fontsize=12)
plt.grid(True)
plt.tight_layout()

plt.savefig("gpu_performance_comparison.png", dpi=300)
plt.savefig("gpu_performance_comparison.svg", dpi=300)
plt.show()