# Ascii sum model
# We train a neural network to predict the sum of the ASCII values of each character in a string.
# Because we think it's a good balance between easy to generate and understand data, but still similar to assembly and the number of cycles per instruction.

import random
import string

import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader

def hash(x : int) -> int:
    return x ^ (x << 13) ^ (x >> 17) ^ (x << 5) % 50

def char_target(c : chr) -> int:
    if c == ' ':
        return 0
    return hash(ord(c) - ord('A') + 1)

MIN_STR_LEN = 20
MAX_STR_LEN = 200
MAX_CHAR_TARGET = 50

def random_string() :
    length = random.randint(MIN_STR_LEN, MAX_STR_LEN)
    s = ""
    for _ in range(length) :
        s += random.choice(string.ascii_letters)
    for _ in range(MAX_STR_LEN - length):
        s += ' '
    return s

def string_to_input(s : str):
    input = []
    for c in s:
        input.append(char_target(c) / MAX_CHAR_TARGET)
    input.append(len(s) / MAX_STR_LEN)
    return input

def str_target(s : str) :
    t = 0
    for c in s :
        t += char_target(c)
    return t / (MAX_CHAR_TARGET * MAX_STR_LEN)

input_size = MAX_STR_LEN + 1
out_size = 1
layers_sizes = [400, 400]

class AsmModel(nn.Module):
    def __init__(self):
        super(AsmModel, self).__init__()
        self.layers = nn.ModuleList()
        for i, size in enumerate(layers_sizes):
            if i == 0:
                self.layers.append(nn.Linear(input_size, size))
            else:
                self.layers.append(nn.Linear(layers_sizes[i-1], size))
            self.layers.append(nn.ReLU())
        self.layers.append(nn.Linear(layers_sizes[-1], out_size))

    def forward(self, x):
        for layer in self.layers:
            x = layer(x)
        return x
    
# Create the model
model = AsmModel()

# Create a DataLoader
inputs = []
inputs_str = []
targets = []
for _ in range(100):
    s = random_string()
    inputs.append(torch.tensor(string_to_input(s), dtype=torch.float32))
    targets.append(torch.tensor([str_target(s)], dtype=torch.float32))
    inputs_str.append(s)
inputs = torch.stack(inputs)
targets = torch.stack(targets)
train_dataset = torch.utils.data.TensorDataset(inputs, targets)
train_loader = DataLoader(dataset=train_dataset, batch_size=4, shuffle=True)

# Hyperparameters
num_epochs = 300
learning_rate = 0.01

# Define loss function and optimizer
criterion = nn.MSELoss()
optimizer = optim.Adam(model.parameters(), lr=learning_rate)

print("Training the model with the following hyperparameters:")
print(f"Number of epochs: {num_epochs}")
print(f"Optimizer: Adam")
print(f"Learning rate: {learning_rate}")
print(f"Loss function: MSELoss")
print(f"Layers sizes: {input_size} -> {layers_sizes} -> {out_size}")

# Training loop
print("Training the model...")
min_loss = 999999.9
best_model = None
loss = 0
for epoch in range(num_epochs):
    for inputs, targets in train_loader:
        # Forward pass
        outputs = model(inputs)
        loss = criterion(outputs, targets)
        
        # Backward pass and optimization
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()
    
    # Save model if it's the best yet
    if loss.item() < min_loss:
        min_loss = loss.item()
        best_model = model.state_dict()
    
    print(f'Epoch {epoch+1}, Loss: {loss.item()/500000:.6f}')

print("Finished training the model.")
# Restore the best model and save it
model.load_state_dict(best_model)
torch.save(model.state_dict(), 'ascii_sum.pth')

print("Testing the model on the training data...")
model.eval()
with torch.no_grad():
    # Map the input str to its predicted target and actual target
    for i in range(len(inputs_str)):
        input_str = inputs_str[i]
        input = string_to_input(input_str)
        input = torch.tensor(input, dtype=torch.float32)
        output = model(input.unsqueeze(0))
        print(f'Input: {input_str}, Predicted: {output.item()}, Actual: {str_target(input_str)}, Difference: {abs(str_target(input_str) - output.item())}')
        

# Testing the model on random data
print("Testing the model on random data...")
for _ in range(10):
    input_str = random_string()
    input = torch.tensor(string_to_input(input_str), dtype=torch.float32)
    with torch.no_grad():
        output = model(input.unsqueeze(0))
    print(f'Input: {input_str}, Predicted: {output.item()}, Actual: {str_target(input_str)}, Difference: {abs(str_target(input_str) - output.item())}')