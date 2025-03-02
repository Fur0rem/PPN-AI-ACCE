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
valid_evol = []

# Parse Epoch: x, Loss: y
for line in file:
    epoch, loss, valid = line.split(",")
    epoch = int(epoch.split(":")[1])
    loss = float(loss.split(":")[1])
    valid = float(valid.split(":")[1])
    epoch_evol.append(epoch)
    loss_evol.append(loss)
    valid_evol.append(valid)


print(loss_evol)

if X_AXIS_LOG:
    plt.xscale("log")
if Y_AXIS_LOG:
    plt.yscale("log")

plt.xlabel("Number of Epochs")
plt.ylabel("Loss")
plt.plot(epoch_evol, loss_evol, label="training loss")
plt.plot(epoch_evol, valid_evol, label="validation loss")
plt.legend()

filename = filepath.split(".")[0]
plt.savefig(filename + ".svg")
plt.savefig(filename + ".png")
