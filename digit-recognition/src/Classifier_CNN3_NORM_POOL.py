import sys
from torch import nn, optim
from Classifier_Baseline import evaluateModel, loadModel

batch_size = 128
layer_stack = nn.Sequential(
    # Conv1
    nn.Conv2d(1, 32, 5, bias=False),
    nn.MaxPool2d(2, 1),
    nn.BatchNorm2d(32),
    nn.ReLU(),
    # Conv2
    nn.Conv2d(32, 64, 5, bias=False),
    nn.MaxPool2d(2, 1),
    nn.BatchNorm2d(64),
    nn.ReLU(),
    # Conv3
    nn.Conv2d(64, 96, 5, bias=False),
    nn.MaxPool2d(2, 1),
    nn.BatchNorm2d(96),
    nn.ReLU(),
    # Output
    nn.Flatten(),
    nn.Linear(13 * 13 * 96, 10, bias=False),
    nn.BatchNorm1d(10),
)
loss_fn = nn.CrossEntropyLoss()
optimizer = optim.Adam
learn_rate = 1e-3
EPOCHS = 30

# evaluate
shouldTrain = True
for x in sys.argv:
    if x == "-load":
        shouldTrain = False
        break
## load state dict instead of training
if not shouldTrain:
    loadModel(layer_stack, "CNN3_NORM_POOL")
## otherwise
else:
    evaluateModel(batch_size, layer_stack, loss_fn, optimizer, learn_rate, EPOCHS)
