import pandas as pd
import matplotlib.pyplot as plt

results = pd.read_csv("training_results/topology_finder.csv")


# Plot the average Test accuracy over generations
def plot_metric_over_time(metric_name: str, bounds: tuple = None):
	"""
	Plot the average metric over generations.
	Args:
		metric_name (str): The name of the metric to plot.
		bounds (tuple, optional): The bounds for the y-axis. Defaults to None.
	"""

	# Get all data grouped by generation
	metric = results.groupby("Generation")[metric_name]

	# Plot the mean accuracy as a line and the range as a filled area
	mean_metric = metric.mean()
	plt.plot(mean_metric, label=f"Average {metric_name}")
	plt.fill_between(mean_metric.index, metric.min(), metric.max(), alpha=0.2, label=f"{metric_name} Range")
	
	# Set the y-axis bounds if provided
	if bounds:
		plt.ylim(bounds)

	# Add the labels and title
	plt.xlabel("Generation")
	plt.ylabel(metric_name)
	plt.title(f"{metric_name} over Generations")
	plt.xticks(mean_metric.index)
	plt.legend()
	plt.show()

# Add a column for the average hidden layer size
results["Mean Hidden Layer Size"] = results["Topology"].apply(
	lambda x: sum([int(i) for i in x.strip().split(" ")]) / len(x.split(" ")))
print("Mean Hidden Layer Size:", results["Mean Hidden Layer Size"])

# Plot the number of hidden layers over generations as a layered filled area
def plot_hidden_layers():
	"""
	Plot the number of hidden layers over generations.
	"""
	# Get the number of hidden layers for each generation like 1: {1: 1, 2: 7, 3: 2, etc.;}
	hidden_layers = results.groupby(["Generation", "Topology"]).size().reset_index(name="Hidden Layers")
	# Convert the topology string to a list of integers
	hidden_layers["Topology"] = hidden_layers["Topology"].apply(
		lambda x: [int(i) for i in x.strip().split(" ")])
	# Get the number of hidden layers
	hidden_layers["Hidden Layers"] = hidden_layers["Topology"].apply(
		lambda x: len(x) - 2 if len(x) > 2 else 0)  # Subtract 1 for the output layer and 1 for the input layer
	
	max_nb_hidden_layers = results["Topology"].apply(
		lambda x: len(x.strip().split(" ")) - 2 if len(x.strip().split(" ")) > 2 else 0).max()
	
	new_dataframe = [[0 for _ in range(max_nb_hidden_layers)] for _ in range(hidden_layers["Generation"].max())]
	for network in results.to_dict(orient="records"):
		gen = network["Generation"]
		topology = network["Topology"]
		# Parse the topology string to a list of integers
		topology = [int(i) for i in topology.strip().split(" ")]
		# Get the number of hidden layers
		nb_hidden_layers = len(topology) - 2 if len(topology) > 2 else 0
		new_dataframe[gen - 1][nb_hidden_layers - 1] += 1
		
	# Transpose the dataframe
	dataframe = pd.DataFrame(columns=["Generation", "Number of Hidden Layers", "Count"])
	for i in range(len(new_dataframe)):
		for j in range(len(new_dataframe[i])):
			dataframe = dataframe._append({"Generation": i + 1, "Number of Hidden Layers": j + 1, "Count": new_dataframe[i][j]}, ignore_index=True)

	# Plot the number of hidden layers as stacked bars
	fig, ax = plt.subplots()
	
	# Compute the proportions
	dataframe["Count"] = dataframe["Count"] / dataframe.groupby("Generation")["Count"].transform("sum")
	dataframe = dataframe.pivot(index="Generation", columns="Number of Hidden Layers", values="Count").fillna(0)
	dataframe.plot(kind="area", stacked=True, ax=ax)
	
	# Have the x-axis be the generations
	ax.set_xlabel("Generation")
	ax.set_title("Proportion of number of hidden layers over generations")
	ax.set_xticks(dataframe.index)
	ax.set_xlim(1, dataframe.index.max())

	
	# Have percentage on the y-axis
	ax.set_ylim(0, 1)
	ax.set_ylabel("Proportion of number of hidden layers")
	ax.set_yticks([i / 10 for i in range(11)])
	ax.set_yticklabels([f"{i * 10}%" for i in range(11)])
	
	ax.legend(title="Number of Hidden Layers")
	plt.xticks(rotation=0)
	plt.show()
	
	
plot_metric_over_time("Train Accuracy", (0.0, 1))
plot_metric_over_time("Validation Accuracy", (0.0, 1))
plot_metric_over_time("Fitness")
plot_metric_over_time("Mean Hidden Layer Size")
plot_hidden_layers()
