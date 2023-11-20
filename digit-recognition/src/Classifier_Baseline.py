import torch
from torch import nn, optim
from torch.utils.data import DataLoader
from torchvision.transforms import ToTensor, Normalize, Compose
from torchvision.datasets import MNIST
import matplotlib.pyplot as plt
import numpy as np
from PIL import Image

# Initialize device
device = "cuda" if torch.cuda.is_available() else "cpu"

# Import train and test data
mean = 0.1307  # this was calculated from training data
std = 0.3081  # this was calculated from training data
target_directory = "assets/"
training_data = MNIST(
    target_directory,
    train=True,
    download=False,
    transform=Compose([ToTensor(), Normalize(mean, std)]),
)
training_data, validation_data = torch.utils.data.random_split(
    training_data, (48000, 12000)
)  # split the data 80/20 for validation and training
test_data = MNIST(
    target_directory,
    train=False,
    download=False,
    transform=Compose([ToTensor(), Normalize(mean, std)]),
)

# Define NeuralNetwork class
class NeuralNetwork(nn.Module):
    def __init__(self, layer_stack: nn.Sequential):
        super(NeuralNetwork, self).__init__()
        self.layer_stack = layer_stack.to(device)

    def forward(self, x):
        logits = self.layer_stack(x)
        return logits


# Train the model using provided loss fn and optimizer
def train(dataloader, model, loss_fn, optimizer):
    model.train()
    train_loss, train_correct = [], []
    for X, y in dataloader:
        X, y = X.to(device), y.to(device)
        # Compute prediction error
        pred = model(X)
        loss = loss_fn(pred, y)
        train_loss.append(loss.item())
        train_correct.extend((pred.argmax(dim=1) == y).type(torch.float).cpu())
        # Backpropagation
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()
    return np.mean(train_loss), np.mean(train_correct)


def test(dataloader, model, loss_fn):
    model.eval()
    test_loss, test_correct = [], []
    with torch.no_grad():
        for X, y in dataloader:
            X, y = X.to(device), y.to(device)
            # Compute prediction error
            pred = model(X)
            loss = loss_fn(pred, y)
            test_loss.append(loss.item())
            test_correct.extend((pred.argmax(dim=1) == y).type(torch.float).cpu())
    return np.mean(test_loss), np.mean(test_correct)


# Initialize default hyperparameters
default = {
    "batch_size": 32,
    "stack": nn.Sequential(nn.Flatten(), nn.Linear(28 * 28, 10)),
    "loss_fn": nn.CrossEntropyLoss(),
    "optimizer": optim.Adam,
    "learn_rate": 1e-3,
    "EPOCHS": 30,
}

# Function to evalute model using different hyperparameters
best_model = None


def evaluateModel(
    batch_size=default["batch_size"],
    layer_stack=default["stack"],
    loss_fn=default["loss_fn"],
    optimizer=default["optimizer"],
    learn_rate=default["learn_rate"],
    EPOCHS=default["EPOCHS"],
):
    global best_model
    # create model using hyper params
    model = NeuralNetwork(layer_stack)
    optimizer = optimizer(model.parameters(), lr=learn_rate)
    print(model)
    # initialize data set with hyper param batch_size
    train_dataloader = DataLoader(training_data, batch_size=batch_size)
    validate_dataloader = DataLoader(validation_data, batch_size=batch_size)
    test_dataloader = DataLoader(test_data, batch_size=batch_size)

    # train and validate model
    best_acc = None
    best_epoch = None
    no_improvement = 5
    t_losses, t_accs, v_losses, v_accs = [], [], [], []

    for epoch in range(1, EPOCHS + 1):
        print(f"Epoch {epoch}\n---------------------------------------------------")

        # train
        train_loss, train_acc = train(train_dataloader, model, loss_fn, optimizer)
        t_losses.append(train_loss), t_accs.append(train_acc * 100)
        print(
            f"Training Loss: {train_loss:>0.3f} \t Training Accuracy: {train_acc*100:>0.2f}%"
        )

        # validate
        val_loss, val_acc = test(validate_dataloader, model, loss_fn)
        v_losses.append(val_loss), v_accs.append(val_acc * 100)
        print(
            f"Validation Loss: {val_loss:>0.3f} \t Validation Accuracy: {val_acc*100:>0.2f}%"
        )
        print("\n")

        # test if we are overfitting
        if best_acc is None or val_acc > best_acc:
            best_acc = val_acc
            best_model = model
            best_epoch = epoch
        if best_epoch + no_improvement <= epoch:
            print(f"No improvement for {no_improvement} epochs!")
            print(f"Best epoch: {best_epoch} \t Epoch Accuracy: {best_acc*100:>0.2f}%")
            break

    fig = plt.figure()
    ax1 = plt.subplot2grid((2, 1), (0, 0))
    ax2 = plt.subplot2grid((2, 1), (1, 0), sharex=ax1)
    fig.suptitle("Comparison of training and validation accuracy/loss")

    ax1.plot(t_accs, label="training accuracy")
    ax1.plot(v_accs, label="validation accuracy")
    ax1.legend(loc=4)
    ax1.set_xlabel("Epochs")
    ax1.set_ylabel("Accuracy (%)")

    ax2.plot(t_losses, label="training loss")
    ax2.plot(v_losses, label="validation loss")
    ax2.legend(loc=1)
    ax2.set_ylabel("Loss")
    ax2.set_xlabel("Epochs")
    plt.show()

    print("Done Training!")
    torch.save(best_model.state_dict(), f"./models/test")
    # Allow user to test model
    receiveInput()


def loadModel(layer_stack, model_name):
    global best_model
    best_model = NeuralNetwork(layer_stack)
    best_model.load_state_dict(
        torch.load(f"./models/{model_name}.pt", map_location=device)
    )
    print("Model Loaded!")
    receiveInput()


def receiveInput():
    path = input("Please enter a filepath:\n> ")
    while path != "exit":
        # open file
        image = Image.open(path)
        x = ToTensor()(image)
        # get prediction of model
        best_model.eval()
        pred = best_model(x.unsqueeze(0))
        print(f"Classifier: {pred.argmax(dim=1).item()}")
        path = input("Please enter a filepath:\n> ")
    print("Exiting...")


# Default call for this file
if __name__ == "__main__":
    evaluateModel()
    receiveInput()
