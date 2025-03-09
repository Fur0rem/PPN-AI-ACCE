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
acc1_evol = []
acc2_evol = []
acc1_evol2 = []
acc2_evol2 = []

# Parse Epoch: x, Loss: y
for line in file:
    epoch, loss, valid, acc1, acc2 = line.split(",")
    epoch = int(epoch.split(":")[1])
    loss = float(loss.split(":")[1])
    valid = float(valid.split(":")[1])
    acc1 = float(acc1.split(":")[1])
    acc2 = float(acc2.split(":")[1])
    epoch_evol.append(epoch)
    loss_evol.append(loss)
    valid_evol.append(valid)
    acc1_evol.append(acc1)
    acc2_evol.append(acc2)
    
    acc1_evol2.append(max(0, acc1))
    
    acc2_evol2.append(max(0, acc2))


print(loss_evol)

### plot loss ###
fig1 = plt.figure()

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
plt.savefig("loss.svg")
plt.savefig("loss.png")

### plot acc ###
fig2 = plt.figure()

# plt.xscale("linear")
# plt.yscale("linear")

plt.xlabel("Number of Epochs")
plt.ylabel("Accuracy")

plt.plot(epoch_evol, acc1_evol, label="training accuracy")
plt.plot(epoch_evol, acc2_evol, label="validation accuracy")

plt.legend()

plt.savefig("acc.svg")
plt.savefig("acc.png")

### plot acc 2 ###
fig2 = plt.figure()

# plt.xscale("linear")
# plt.yscale("linear")

plt.xlabel("Number of Epochs")
plt.ylabel("Accuracy")

plt.plot(epoch_evol, acc1_evol2, label="training accuracy")
plt.plot(epoch_evol, acc2_evol2, label="validation accuracy")

plt.legend()

plt.savefig("acc2.svg")
plt.savefig("acc2.png")