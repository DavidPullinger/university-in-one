# falling-words

## Description

A simple Java game where the player must enter words in a text field before they hit the bottom of the screen. Each word's animation/state is independently controlled by a single thread.

## Execution error: InputMismatchException

If there is an input mismatch exception error, it is most likely because the numbers in the
data/scores.txt are formatted with a '.' as a comma separator instead of a ','. Please replace these appropriately
and it should run fine.

## Compile error: SymbolNotFound

If there is a SymbolNotFound error, this is because of my Makefile. Please run `javac -d bin/ src/*.java` to recompile the classes this way. The application can then be run normally.

## Running the game

You can run the game by executing `make run totalWords=x noWords=y dict=z` in the root directory of this project, where `x`, `y` and `z` are mandatory parameters: `x` and `y` are integers while `z` is a path to a text file with words for the game
