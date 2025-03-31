import sys
import matplotlib.pyplot as plt

X_AXIS_LOG = False
Y_AXIS_LOG = False

if (len(sys.argv) < 2):
    print("use format:\npython3 data.py [filepath]")
    exit()

filepath = sys.argv[1]

file = open(filepath, "r")
epoch_list, info = file.read().split("Training time")
epoch_list = epoch_list[:-1]
epoch_list = epoch_list.split('\n')
epoch_list = [line.split(",") for line in epoch_list]


names = [elem.split(":")[0] for elem in epoch_list[0]]
values = [[] for line in names]

for line in epoch_list:
    for i in range(len(values)):
        values[i].append(float(line[i].split(":")[1]))

values[0] = [int(elem) for elem in values[0]]


### plot accuracy ###
fig1, acc_ax = plt.subplots()


if X_AXIS_LOG:
    plt.xscale("log")
if Y_AXIS_LOG:
    plt.yscale("log")

plt.xlabel("Number of Epochs")
acc_ax.set_ylabel("Accuracy")
acc_ax.set(ylim=(0.0, 1.0))


x_axis = values[0]

for i in range(1, len(values)):
    if "MRAE" in names[i]:
        acc_ax.plot( values[i], label=names[i])

plt.legend()

filename = filepath.split(".")[0]
plt.savefig("accuracy.svg")
plt.savefig("accuracy.png")
