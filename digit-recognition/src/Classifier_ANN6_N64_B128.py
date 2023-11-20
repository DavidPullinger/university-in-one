from torch import nn, optim
from Classifier_Baseline import evaluateModel


batch_size = 128
layer_stack = nn.Sequential(
    # Flatten input
    nn.Flatten(),
    # Linear 1
    nn.Linear(28 * 28, 64),
    nn.ReLU(),
    # Linear 2
    nn.Linear(64, 64),
    nn.ReLU(),
    # Linear 3
    nn.Linear(64, 64),
    nn.ReLU(),
    # Linear 4
    nn.Linear(64, 64),
    nn.ReLU(),
    # Linear 5
    nn.Linear(64, 64),
    nn.ReLU(),
    # Linear 6
    nn.Linear(64, 64),
    nn.ReLU(),
    # Output
    nn.Linear(64, 10),
)
loss_fn = nn.CrossEntropyLoss()
optimizer = optim.Adam
learn_rate = 1e-3
EPOCHS = 30

# evaluate
evaluateModel(batch_size, layer_stack, loss_fn, optimizer, learn_rate, EPOCHS)
