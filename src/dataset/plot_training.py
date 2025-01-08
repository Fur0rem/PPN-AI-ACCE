import sys
import matplotlib.pyplot as plt

X_AXIS_LOG = False
Y_AXIS_LOG = True

if (len(sys.argv) < 2):
    print("use format:\npython3 data.py [filepath]")
    exit()

filepath = sys.argv[1]

file = open(filepath, "r")
epoch_evol = []
loss_evol = []

# Parse Epoch: x, Loss: y
for line in file:
    epoch, loss = line.split(",")
    epoch = int(epoch.split(":")[1])
    loss = float(loss.split(":")[1])
    epoch_evol.append(epoch)
    loss_evol.append(loss)


print(loss_evol)

if X_AXIS_LOG:
    plt.xscale("log")
if Y_AXIS_LOG:
    plt.yscale("log")

plt.xlabel("Number of Epochs")
plt.ylabel("Loss")
plt.plot(epoch_evol, loss_evol)

filename = filepath.split(".")[0]
plt.savefig(filename + ".svg")
plt.savefig(filename + ".png")
