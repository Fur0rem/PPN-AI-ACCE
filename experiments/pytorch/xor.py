import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader

input_size = 2
out_size = 1
layers_sizes = [8]

class XorModel(nn.Module):
    def __init__(self):
        super(XorModel, self).__init__()
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
model = XorModel()

# Create a DataLoader
inputs = torch.tensor([[0, 0], [0, 1], [1, 0], [1, 1]], dtype=torch.float32)
targets = torch.tensor([[0], [1], [1], [0]], dtype=torch.float32)
train_dataset = torch.utils.data.TensorDataset(inputs, targets)
train_loader = DataLoader(dataset=train_dataset, batch_size=4, shuffle=True)

# Define loss function and optimizer
criterion = nn.MSELoss()
optimizer = optim.Adam(model.parameters(), lr=0.1)

# Hyperparameters
num_epochs = 200

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
    
    print(f'Epoch [{epoch+1}/{num_epochs}], Loss: {loss.item():.4f}')

print("Finished training the model.")

# Restore the best model and save it
model.load_state_dict(best_model)
torch.save(model.state_dict(), 'xor_best_model.pth')

# Testing the model on the training data
print("Testing the model on the training data...")
model.eval()
with torch.no_grad():
    for inputs, targets in train_loader:
        outputs = model(inputs)
        for target, output in zip(targets, outputs):
            print(f'Target: {target.item()}, Predicted: {output.item()}, Difference: {abs(target.item() - output.item())}')

# Testing the model on random data
print("Testing the model on random data...")
for _ in range(10):
    input = torch.randint(0, 2, (2,), dtype=torch.float32)
    with torch.no_grad():
        output = model(input.unsqueeze(0))
    expected = 1 if input[0] != input[1] else 0
    print(f'Input: {input.numpy()}, Target: {expected}, Predicted: {output.item()}, Difference: {abs(expected - output.item())}')