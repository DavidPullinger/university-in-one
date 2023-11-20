from FourRooms import FourRooms
from tqdm import tqdm
import numpy as np
import sys

# constants
LEARNING_RATE = 0.1
DISCOUNT = 0.999
EPOCHS = 1000
EPSILON = 1
MIN_EPSILON = 0.00000001
EPSILON_DECAY_FACTOR = MIN_EPSILON ** (1 / EPOCHS)

# table of q values for agent
ACTIONS = 4
X = Y = 13
PACKAGES = 3
q_table = np.zeros((X, Y, PACKAGES, ACTIONS))


def calc_stochastic_q(action, currentPos, packsLeft):
    q_sum = 0
    unintended_actions = [0, 1, 2, 3]
    unintended_actions.remove(action)
    for unintended_act in unintended_actions:
        q_sum += (
            0.8 * q_table[currentPos][packsLeft - 1][action]
            + 0.2 * q_table[currentPos][packsLeft - 1][unintended_act]
        )
    # return average "stochastic q" for this actions
    return q_sum / 3


def main():
    stochastic = len(sys.argv) > 1 and sys.argv[1] == "-stochastic"
    # create FourRooms Object
    fourRoomsObj = FourRooms("multi", stochastic)
    # train agent
    global EPSILON

    for epoch in tqdm(range(EPOCHS)):
        # reset env
        fourRoomsObj.newEpoch()
        # loop until we get to terminal state
        while True:
            currentPos = fourRoomsObj.getPosition()
            currentPackagesLeft = fourRoomsObj.getPackagesRemaining()
            # get e-greedy action with probability 1-e
            if np.random.random() > EPSILON:
                # if actions are not stochastic, we take one with max q
                if not stochastic:
                    action = np.argmax(q_table[currentPos][currentPackagesLeft - 1])
                # if they are, we use the probability of each action to choose max
                else:
                    action = 0
                    for act in range(ACTIONS):
                        if calc_stochastic_q(
                            act, currentPos, currentPackagesLeft
                        ) > calc_stochastic_q(action, currentPos, currentPackagesLeft):
                            action = act
            else:
                # get random action
                action = np.random.randint(0, 4)

            # execute actions and observe reward and new state
            cellType, newPos, newPackagesLeft, isTerminal = fourRoomsObj.takeAction(
                action
            )
            # +1 reward if we reached terminal state
            # -0.001 reward for a step not into terminal state
            # -0.01 reward for a step into a wall
            reward = (
                isTerminal - 0.001 * (not isTerminal) - 0.01 * (newPos == currentPos)
            )

            if not isTerminal:
                # max possible Q value in next step
                max_future_q = np.max(q_table[newPos][newPackagesLeft - 1])

                # current Q value
                current_q = q_table[currentPos][currentPackagesLeft - 1][action]

                # calculate new q
                new_q = (1 - LEARNING_RATE) * current_q + LEARNING_RATE * (
                    reward + DISCOUNT * max_future_q
                )
                # update Q table with new Q value
                q_table[currentPos][currentPackagesLeft - 1][action] = new_q
            else:
                q_table[currentPos][currentPackagesLeft - 1][action] = reward
                break
        # decay epsilon
        EPSILON *= EPSILON_DECAY_FACTOR
        if epoch + 5 >= EPOCHS:
            EPSILON = 0
    # view path
    fourRoomsObj.showPath(-1)


if __name__ == "__main__":
    main()
