import matplotlib.pyplot as plt
import numpy as np

results = {
	"Baseline": {
		"Training": 0.53,
		"Validation": 0.15,
	},
	"Encoding log2": {
		"Training": 0.70,
		"Validation": 0.33,
	},
	"Mini-Batch + Adam": {
		"Training": 0.89,
		"Validation": 0.15,
	},
	"Evolved Topology": {
		"Training": 0.84,
		"Validation": 0.37,
	},
}

def plot_comparaison(all_names: list[str]):
	training_acc = [results[name]["Training"] for name in all_names]
	validation_acc = [results[name]["Validation"] for name in all_names]

	x = np.arange(len(all_names))
	width = 0.35

	fig, ax = plt.subplots()
	rects1 = ax.bar(x - width/2, training_acc, width, label='Training', color='blue')
	rects2 = ax.bar(x + width/2, validation_acc, width, label='Validation', color='orange')

	# Add accuracy values on top of the bars
	def add_labels(rects):
		for rect in rects:
			height = rect.get_height()
			ax.text(
			rect.get_x() + rect.get_width() / 2, 
			height, 
			f'{height:.2f}', 
			ha='center', 
			va='bottom'
			)

	add_labels(rects1)
	add_labels(rects2)

	ax.set_ylabel('Accuracy', fontsize=14)
	ax.set_xlabel('Optimisation Method', fontsize=16)
	ax.set_xticks(x)
	ax.tick_params(axis='both', which='major', labelsize=10, rotation=20)
	ax.set_xticklabels(all_names)
	ax.legend()

	fig.tight_layout()
	plt.savefig("comparaison_accuracy.svg", dpi=300, bbox_inches='tight')
	plt.show()

plot_comparaison(["Baseline", "Encoding log2", "Mini-Batch + Adam", "Evolved Topology"])
