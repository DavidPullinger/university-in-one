import sys
from torch import nn, optim
from Classifier_Baseline import evaluateModel, loadModel

batch_size = 128
layer_stack = nn.Sequential(
    # Conv1
    nn.Conv2d(1, 32, 3, bias=False),
    nn.ReLU(),
    # Conv2
    nn.Conv2d(32, 48, 3, bias=False),
    nn.ReLU(),
    # Conv3
    nn.Conv2d(48, 64, 3, bias=False),
    nn.ReLU(),
    # Output
    nn.Flatten(),
    nn.Linear(22 * 22 * 64, 10),
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
    loadModel(layer_stack, "CNN3")
## otherwise
else:
    evaluateModel(batch_size, layer_stack, loss_fn, optimizer, learn_rate, EPOCHS)
