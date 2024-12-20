import sys
import matplotlib.pyplot as plt

X_AXIS_LOG = True
Y_AXIS_LOG = True

if (len(sys.argv) < 2):
    print("use format:\npython3 data.py [filepath]")
    exit()

filepath = sys.argv[1]

file = open(filepath, "r")
epoch = []
loss = []

line = file.readline()
while (line):
    line = line[:-1]
    line = line.replace(" ", "")
    line = line.split(",")
    epoch.append(int(line[0].split(":")[1]))
    loss.append(float(line[1].split(":")[1]))
    line = file.readline()

if X_AXIS_LOG:
    plt.xscale("log")
if Y_AXIS_LOG:
    plt.yscale("log")

plt.xlabel("Epoch")
plt.ylabel("Loss")

plt.plot(epoch, loss)
plt.show()