# Hand-written Digit Recognition using Artificial and Convolutional Neural Networks

## Prerequisites

The prerequisite libraries used in this Python project can be installed by running `make` in the root directory of this project. Please ensure that you run `make clean` before doing so.

It is also required that the MNIST training data files be in a folder names `assets/` in the root directory of this project.

## Execution of ANNs

To execute the Baseline "ANN" model (a simple linear regression model), run `make run` in the root directory of this project. To execute a specific ANN, run `python3 src/ANN_suffix` from the root directory of this project, where `suffix` corresponds to the parameters that the model is initialized with. These parameters are:

1. Number of hidden layers (e.g. ANN6 corresponds to 6 hidden layers)
2. Number of neurons in each layer (e.g. N64 corresponds to 64 neurons in each layer)
3. Batch size (e.b. B256 corresponds to a batch size of 256)

Together, these 3 parameters make a file name. For instance, `Classifier_ANN3_N256_B128.py` corresponds to an ANN with 3 layers of 256 neurons each, which is trained using a batch size of 128

Please note that there are also two ANNs which differ from the rest in that they do not have a fixed number of neurons per layer. These are:

1. `Classifier_ANN3_ASC` (3 hidden layer ANN with neurons/layer 64 -> 128 -> 256)
2. `Classifier_ANN3_DESC` (3 hidden layer ANN with neurons/layer 256 -> 128 -> 64)

## Execution of CNNs

Since these neural networks take long to train, I have included their state dictionaries (list of parameters) under `models/`. Therefore, if you want to skip the training stage, append the flag `-load` when calling any of the CNNs. (e.g. `python3 src/Classifier_CNN3_NORM.py -load`).

The CNNs have a similar file naming convention as the ANNs. The various parameters when calling any of the CNNs are:

1. Whether there are batch normalization layers (NORM indicates there are)
2. Whether there are max pooling layers (POOL indicates there are)
3. Whether there is dropout (DROPOUT indicates there is)

Please note that the `CNN3_DROPOUT` and `CNN3_NORM_POOL` classifiers are not working as expected. When training, they indicated a validation accuracy of greater than 99%, but 90% of user supplied input is classified as a 1. I have not been able to figure out why (likely cause is over-fitting, but the validation set does not reflect this).
