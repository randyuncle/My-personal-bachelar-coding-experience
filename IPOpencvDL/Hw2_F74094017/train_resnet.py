# %%
import os
import torch
import torch.nn as nn
import torchvision.models as models
import torch.optim as optim
import torchsummary
from torch.utils.data import DataLoader
from torch.utils.data import Dataset
import torchvision.transforms as transforms
from torchvision import datasets
from PIL import Image, UnidentifiedImageError

# Data transformation pipeline with Random Erasing
transform_with_erasing = transforms.Compose([
    transforms.Resize((224, 224)),
    transforms.RandomHorizontalFlip(),
    transforms.RandomVerticalFlip(),
    transforms.ToTensor(),
    transforms.RandomErasing()  # Add Random Erasing here
])

# Data transformation pipeline without Random Erasing
transform_without_erasing = transforms.Compose([
    transforms.Resize((224, 224)),
    transforms.RandomHorizontalFlip(),
    transforms.RandomVerticalFlip(),
    transforms.ToTensor(),
])

# %%
# Build ResNet50 model
def resnet50_custom(num_classes=1):
    resnet50 = models.resnet50()
    
    # Replace the output layer
    in_features = resnet50.fc.in_features
    resnet50.fc = nn.Sequential(
        nn.Linear(in_features, num_classes),
        nn.Sigmoid()
    )
    
    return resnet50

# %%
training_dataset_path = 'Dataset_OpenCvDl_Hw2_Q5/dataset/training_dataset'
# Load training data with DataLoader
train_dataset = datasets.ImageFolder(training_dataset_path, transform_without_erasing)
train_loader = DataLoader(train_dataset, batch_size=64, shuffle=True)

# %%
validation_dataset_path = 'Dataset_OpenCvDl_Hw2_Q5/dataset/validation_dataset'
# Load validation data with DataLoader
validation_dataset = datasets.ImageFolder(validation_dataset_path, transform_without_erasing)
validation_loader = DataLoader(validation_dataset, batch_size=64, shuffle=True)

# %%
# Initialize the model
model_w_n_erase = resnet50_custom()

# Define loss function and optimizer
criterion = nn.BCELoss()
optimizer = optim.Adam(model_w_n_erase.parameters(), lr=0.001)

#torchsummary.summary(model_w_n_erase, (3, 224, 224)) 

# %%
# Training loop
num_epochs = 60
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
model_w_n_erase.to(device)

train_loss_history = []
train_accuracy_history = []
val_loss_history = []
val_accuracy_history = []
best_without_erase_val_accuracy = 0.0

for epoch in range(num_epochs):
    model_w_n_erase.train()

    running_loss = 0.0
    correct_train = 0
    total_train = 0
        
    for inputs, labels in train_loader:
        inputs, labels = inputs.to(device), labels.to(device)

        optimizer.zero_grad()

        outputs = model_w_n_erase(inputs)
        loss = criterion(outputs, labels.unsqueeze(1).float())  # labels are 0 or 1

        loss.backward()
        optimizer.step()
        
        running_loss += loss.item()

        predicted = (outputs > 0.5).float()
        total_train += labels.size(0)
        correct_train += (predicted == labels.view(-1, 1)).sum().item()
        
    train_accuracy = 100 * correct_train / total_train
    train_loss = running_loss / len(train_loader)
    train_loss_history.append(train_loss)
    train_accuracy_history.append(train_accuracy)
    
    model_w_n_erase.eval()
    val_loss = 0.0
    correct_val = 0
    total_val = 0
    
    with torch.no_grad():
        for data in validation_loader:
            inputs, labels = data
            inputs, labels = inputs.to(device), labels.to(device)

            outputs = model_w_n_erase(inputs)
            loss = criterion(outputs, labels.unsqueeze(1).float())

            val_loss += loss.item()

            predicted = (outputs > 0.5).float()
            total_val += labels.size(0)
            correct_val += (predicted == labels.view(-1, 1)).sum().item()

    val_accuracy = 100 * correct_val / total_val
    val_loss = val_loss / len(validation_loader)
    val_loss_history.append(val_loss)
    val_accuracy_history.append(val_accuracy)
    
    print(f'Epoch [{epoch + 1}/{num_epochs}] | '
            f'Train Loss: {train_loss:.4f} | Train Accuracy: {train_accuracy:.2f}% | '
            f'Validation Loss: {val_loss:.4f} | Validation Accuracy: {val_accuracy:.2f}%')

    if val_accuracy >= best_without_erase_val_accuracy:
        best_without_erase_val_accuracy = val_accuracy
        torch.save(model_w_n_erase.state_dict(), "resnet50_model_without_erase.pth")


# %%
# Load training data with DataLoader
train_erase_dataset = datasets.ImageFolder(training_dataset_path, transform_with_erasing)
train_erase_loader = DataLoader(train_erase_dataset, batch_size=64, shuffle=True)

# %%
# Initialize the model
model_w_erase = resnet50_custom()

# Define loss function and optimizer
criterion = nn.BCELoss()
optimizer = optim.Adam(model_w_erase.parameters(), lr=0.001)

# %%
# Training loop
num_epochs = 60
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
model_w_erase.to(device)

train_loss_history = []
train_accuracy_history = []
val_loss_history = []
val_accuracy_history = []
best_erase_val_accuracy = 0.0

for epoch in range(num_epochs):
    model_w_erase.train()

    running_loss = 0.0
    correct_train = 0
    total_train = 0
        
    for inputs, labels in train_erase_loader:
        inputs, labels = inputs.to(device), labels.to(device)

        optimizer.zero_grad()

        outputs = model_w_erase(inputs)
        loss = criterion(outputs, labels.unsqueeze(1).float())  # labels are 0 or 1

        loss.backward()
        optimizer.step()
        
        running_loss += loss.item()

        predicted = (outputs > 0.5).float()
        total_train += labels.size(0)
        correct_train += (predicted == labels.view(-1, 1)).sum().item()
        
    train_accuracy = 100 * correct_train / total_train
    train_loss = running_loss / len(train_erase_loader)
    train_loss_history.append(train_loss)
    train_accuracy_history.append(train_accuracy)
    
    model_w_erase.eval()
    val_loss = 0.0
    correct_val = 0
    total_val = 0
    
    with torch.no_grad():
        for data in validation_loader:
            inputs, labels = data
            inputs, labels = inputs.to(device), labels.to(device)

            outputs = model_w_erase(inputs)
            loss = criterion(outputs, labels.unsqueeze(1).float())

            val_loss += loss.item()

            predicted = (outputs > 0.5).float()
            total_val += labels.size(0)
            correct_val += (predicted == labels.view(-1, 1)).sum().item()

    val_accuracy = 100 * correct_val / total_val
    val_loss = val_loss / len(validation_loader)
    val_loss_history.append(val_loss)
    val_accuracy_history.append(val_accuracy)
    
    print(f'Epoch [{epoch + 1}/{num_epochs}] | '
            f'Train Loss: {train_loss:.4f} | Train Accuracy: {train_accuracy:.2f}% | '
            f'Validation Loss: {val_loss:.4f} | Validation Accuracy: {val_accuracy:.2f}%')

    if val_accuracy >= best_erase_val_accuracy:
        best_erase_val_accuracy = val_accuracy
        torch.save(model_w_erase.state_dict(), "resnet50_model_with_erase.pth")
        

# %%
import matplotlib.pyplot as plt

# Bar chart labels and values
models = ['Without Erase', 'With Erase']
accuracies = [best_without_erase_val_accuracy, best_erase_val_accuracy]

# Plotting the bar chart
plt.bar(models, accuracies, color=['blue', 'green'])

# Adding labels and title
plt.xlabel('Models')
plt.ylabel('Validation Accuracy')
plt.title('Comparison of Validation Accuracies')

# Displaying the values on top of the bars
for i, v in enumerate(accuracies):
    plt.text(i, v + 0.02, str(round(v, 2)), ha='center', va='bottom')

# Save the figure
plt.savefig('validation_accuracies_comparison.png')

# Show the plot
plt.show()


