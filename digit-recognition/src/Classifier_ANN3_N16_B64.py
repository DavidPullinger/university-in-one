from torch import nn, optim
from Classifier_Baseline import evaluateModel


batch_size = 64
layer_stack = nn.Sequential(
    # Flatten input
    nn.Flatten(),
    # Linear 1
    nn.Linear(28 * 28, 16),
    nn.ReLU(),
    # Linear 2
    nn.Linear(16, 16),
    nn.ReLU(),
    # Linear 3
    nn.Linear(16, 16),
    nn.ReLU(),
    # Output
    nn.Linear(16, 10),
)
loss_fn = nn.CrossEntropyLoss()
optimizer = optim.Adam
learn_rate = 1e-3
EPOCHS = 30

# evaluate
evaluateModel(batch_size, layer_stack, loss_fn, optimizer, learn_rate, EPOCHS)
