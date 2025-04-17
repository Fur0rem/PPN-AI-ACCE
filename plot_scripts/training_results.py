training = ""
reference = ""

import sys

if (len(sys.argv) < 2):
    print("Use: python3 training_results.py [New result directory] [Reference result directory] (Optional)")
    exit()

training = sys.argv[1]
if (len(sys.argv) > 2):
    reference = sys.argv[2]
    
# Read the training results
from matplotlib import pyplot as plt
from matplotlib.gridspec import GridSpec
import pandas as pd
import os

def load_results(directory: str) -> tuple[list[dict], list[float]]:
    """
    Load the training results from the specified directory.
    Returns a tuple containing a list of dictionaries with the training results and a list of training times.
    """
    results = []
    training_times = []
    print(f"Loading results from {directory}")
    for filename in os.listdir(directory):
        if filename.endswith(".log"):
            filepath = os.path.join(directory, filename)
            results.append([])
            with open(filepath, "r") as file:
                lines = file.readlines()
                # Extract the relevant information from the lines
                for line in lines:
                    # Epoch: 99	,Training loss (MRSE): 0.132272	,Validation loss (MRSE): 0.813506	,Training loss (MSE): 2.50187e+17	,Validation loss (MSE): 4.71511e+13	,Training accuracy (MAE): -8.69462e+07	,Validation accuracy (MAE): -3.06477e+06	,Training accuracy (MRAE): 0.752687	,Validation accuracy (MRAE): 0.128448
                    # Training time: 27.8775s
                    if "Training time" in line:
                        parts = line.split(":")
                        training_time = float(parts[1].split("s")[0])
                        training_times.append(training_time)
                    # Epoch: 0,	Training loss (MRSE): 0.265653	,Validation loss (MRSE): -nan	,Training loss (MSE): 0.265653	,Validation loss (MSE): -nan	,Training accuracy (MAE): 0.499707	,Validation accuracy (MAE): -nan	,Training accuracy (MRAE): 0.499707	,Validation accuracy (MRAE): -nan 
                    if "Epoch" in line:
                        parts = line.split(",")
                        epoch = int(parts[0].split(":")[1])
                        training_accuracy_mrae = float(parts[-2].split(":")[1])
                        validation_accuracy_mrae = float(parts[-1].split(":")[1])
                        results[-1].append({
                            "Epoch": epoch,
                            "Training accuracy (MRAE)": training_accuracy_mrae,
                            "Validation accuracy (MRAE)": validation_accuracy_mrae
                        })

    return results, training_times
    
new_results = load_results(training)
reference_results = None
if reference:
    reference_results = load_results(reference)

print(new_results[0][0][-1])
print(reference_results[0][0][-1])
                    
def mean_min_max_per_epoch(results: list[dict], key: str) -> pd.DataFrame:
    """
    Calculate the mean, min, and max of the training and validation accuracies per epoch.
    """
    # Convert the list of dictionaries to a DataFrame
    flattened_results = [item for sublist in results for item in sublist]
    df = pd.DataFrame(flattened_results)
    
    # Group by epoch and calculate mean, min, and max
    grouped = df.groupby("Epoch").agg(
        Mean=(key, "mean"),
        Min=(key, "min"),
        Max=(key, "max")
    ).reset_index()
    
    return grouped

# Calculate mean, min, and max for "Training accuracy (MRAE)"
new_train_acc = mean_min_max_per_epoch(new_results[0], "Training accuracy (MRAE)")
new_valid_acc = mean_min_max_per_epoch(new_results[0], "Validation accuracy (MRAE)")
new_times = new_results[1]
reference_train_acc = None
reference_valid_acc = None
reference_times = None
if reference_results:
    reference_train_acc = mean_min_max_per_epoch(reference_results[0], "Training accuracy (MRAE)")
    reference_valid_acc = mean_min_max_per_epoch(reference_results[0], "Validation accuracy (MRAE)")
    reference_times = reference_results[1]

# Plot the training and validation accuracies using a plot with shaded areas
def plot_accuracy_over_epochs(new_train_acc: pd.DataFrame, new_valid_acc: pd.DataFrame, new_train_times: list[float], title: str,
                              ref_train_acc: pd.DataFrame = None, ref_valid_acc: pd.DataFrame = None, ref_train_times: list[float] = None):
    """
    Plot the training and validation accuracies over epochs. With the reference accuracies if provided.
    """

    # Put some information at the bottom of the plot (accs of the last epoch for training and reference, training time for training and reference)
    if ref_train_acc is not None and ref_valid_acc is not None:
        # Do the average of the training times with relative error
        training_times_avg = sum(new_train_times) / len(new_train_times)
        reference_times_avg = sum(ref_train_times) / len(ref_train_times)
        training_times_std = (sum([(x - training_times_avg) ** 2 for x in new_train_times]) / len(new_train_times)) ** 0.5
        reference_times_std = (sum([(x - reference_times_avg) ** 2 for x in ref_train_times]) / len(ref_train_times)) ** 0.5
        # Get the last accuracies of the training and reference
        ref_train_acc_last = ref_train_acc["Mean"].iloc[-1]
        ref_valid_acc_last = ref_valid_acc["Mean"].iloc[-1]
        train_train_acc_last = new_train_acc["Mean"].iloc[-1]
        train_valid_acc_last = new_valid_acc["Mean"].iloc[-1]

        # Compare the two training times
        times_diff = training_times_avg - reference_times_avg
        train_acc_diff = train_train_acc_last - ref_train_acc_last
        valid_acc_diff = train_valid_acc_last - ref_valid_acc_last

    # Create a figure with a grid layout
    fig = plt.figure(figsize=(10, 8))
    gs = GridSpec(2, 1, height_ratios=[3, 1])  # 3:1 ratio for plot and table

    # Plot (top section)
    ax_plot = fig.add_subplot(gs[0])
    ax_plot.set_ylim(0, 1)

    # Plot training accuracy
    ax_plot.plot(new_train_acc["Epoch"], new_train_acc["Mean"], label="New Training Accuracy", color="blue")
    ax_plot.fill_between(new_train_acc["Epoch"], new_train_acc["Min"], new_train_acc["Max"], color="blue", alpha=0.33)

    # Plot validation accuracy
    ax_plot.plot(new_valid_acc["Epoch"], new_valid_acc["Mean"], label="New Validation Accuracy", color="orange")
    ax_plot.fill_between(new_valid_acc["Epoch"], new_valid_acc["Min"], new_valid_acc["Max"], color="orange", alpha=0.33)

    # Add reference lines
    ax_plot.axhline(y=ref_train_acc_last, color="#1e66a3", linestyle="-.", label="Reference Training Accuracy")
    ax_plot.axhline(y=ref_valid_acc_last, color="#bb5e0d", linestyle="-.", label="Reference Validation Accuracy")

    training_name = os.path.basename(training)
    reference_name = os.path.basename(reference) if reference else "N/A"
    
    # Add labels, title, and legend
    ax_plot.set_xlabel("Epoch")
    ax_plot.set_ylabel("Accuracy (MRAE)")
    ax_plot.set_title(f"Training and Validation accuracy over epochs for New ({training_name}) and Reference ({reference_name}) models")
    ax_plot.legend()

    # Table (bottom section)
    ax_table = fig.add_subplot(gs[1])
    ax_table.axis("off")  # Turn off the axis for the table

    # Table data
    table_data = [
        ["Metric", "New", "Reference", "Difference"],
        ["Training time", f"{training_times_avg:.2f}s ± {training_times_std:.2f}s", f"{reference_times_avg:.2f}s ± {reference_times_std:.2f}s", f"{times_diff:+.2f}s"],
        ["Training accuracy", f"{train_train_acc_last:.2f}", f"{ref_train_acc_last:.2f}", f"{train_acc_diff:+.2f}"],
        ["Validation accuracy", f"{train_valid_acc_last:.2f}", f"{ref_valid_acc_last:.2f}", f"{valid_acc_diff:+.2f}"],
    ]

    # Add the table to the plot
    table = ax_table.table(
        cellText=table_data,
        colLabels=None,
        cellLoc="center",
        loc="center",
    )

    ax_table.text(
        0.5,
        0.9,
        "Summary of Training and Validation results",
        ha="center",
        va="center",
        fontsize=14,
        transform=ax_table.transAxes,
    )

    # Style the table
    table.auto_set_font_size(False)
    table.set_fontsize(10)
    table.auto_set_column_width(col=list(range(len(table_data[0]))))

    # Highlight positive/negative differences
    for i in range(1, len(table_data)):
        is_train_time = table_data[i][0] == "Training time"
        if is_train_time:
            diff = float(table_data[i][3][1:-2]) # Extract the difference value and the s suffix
        else:
            diff = float(table_data[i][3][1:])
        color = "black"
        if is_train_time:
            color = "green" if diff < 0 else "red"
        else:
            color = "green" if diff > 0 else "red"
        table[(i, 3)].set_text_props(color=color)
    # Adjust layout and show the plot
    plt.tight_layout()
    plt.show()
    
# Plot the training and validation accuracies
plot_accuracy_over_epochs(new_train_acc, new_valid_acc, new_times, "Training and Validation Accuracies over Epochs", reference_train_acc, reference_valid_acc, reference_times)