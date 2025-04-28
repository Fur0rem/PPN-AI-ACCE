from matplotlib import pyplot as plt

### FIGURE 1 PROPORTIONS ###

# read file
file_proportion = open("kernel_zeros_proportion.txt", "r")
text = file_proportion.read()

# extract data
bins = [x * 10 for x in range(10+1)]
zeros = [float(x) * 100 for x in (text.split("\n")[:-1])]

# plot data
plt.figure(1)

plt.hist(zeros, bins=bins)
plt.xlabel("Proportion of useless data ( % )")
plt.ylabel("Number of kernels")
plt.xticks(ticks=bins)


### FIGURE 2 RAW NUMBERS ###

# read file
file_raw = open("kernel_zeros_raw.txt", "r")
text = file_raw.read()

# extract data
zeros = [int(x) for x in (text.split("\n")[1:-1])]
max_ = int(text.split("\n")[0])
bins = [x * max_ / 10 for x in range(10+1)]

# plot data
plt.figure(2)

plt.hist(zeros, bins=10, range=(0, max_))
plt.xlabel("Number of useless data")
plt.ylabel("Number of kernels")
plt.xticks(ticks=bins)


# close files
file_proportion.close()
file_raw.close()

plt.show()