# Training an RL Agent with Q-Learning

## Installation and execution

You can create a virtual environment and install the project dependencies by running `make install`. To run the RL Agent (in Scenario 1), run `make run`.
The 3 scenarios can be executed by running `python3 src/ScenarioN.py` from the root directory where `N` is one of `1`,`2`, or `3`.

## Stochasticity

All RL Agents can be executed in a stochastic environment, where an intended action only has an 80% chance of occurring. The RL Agents can be executed in such an environment by adding the `-stochastic` flag where necessary (e.g. `python3 src/Scenario1.py -stochastic`)

## Scenarios

The RL Agents were trained via Q-Learning for all scenarios. The scenarios and the agents' parameters and hyperparameters are described below.

### Scenario 1

In Scenario 1, the agent must simply retrieve a single package, via the shortest path. It was trained with the following parameters and hyperparameters:

1. Epsilon: Decayed exponentially from 1 to 0
2. Discount: 0.99
3. Learning rate: 0.1
4. Epochs: 1000
5. State: Current position of Agent
6. Reward:
   - +100 reward if we reached terminal state
   - -1 reward for a step not into terminal state
   - -1 reward for a step into a wall

### Scenario 2

In Scenario 2, the agent had to retrieve 3 packages, in any order, via the shortest path. It was trained with the following parameters and hyperparameters:

1. Epsilon: Decayed exponentially from 1 to 0
2. Discount: 0.999
3. Learning rate: 0.1
4. Epochs: 1000
5. State: Current position of Agent and number of packages remaining
6. Reward:
   - +1 reward if we reached terminal state
   - -0.001 reward for a step not into terminal state
   - -0.01 reward for a step into a wall

### Scenario 3

In Scenario 3, the agent had to retrieve 3 packages, in the order Red, Green, Blue, via the shortest path. It was trained with the following parameters and hyperparameters:

1. Epsilon: Decayed exponentially from 1 to 0
2. Discount: 0.999
3. Learning rate: 0.1
4. Epochs: 1000
5. State: Current position of Agent and number of packages remaining
6. Reward:
   - +1 reward if we reached terminal state correctly (by picking up packages in the order Red, Green, Blue)
   - -1 reward if we reached terminal state incorrectly (by picking up packages in the wrong order)
   - -0.001 reward for a step not into terminal state
   - -0.01 reward for a step into a wall
