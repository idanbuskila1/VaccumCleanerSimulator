# Advanced Topics In Programming: Assignment 1
## Brief Description
The aproach was to model the problem into 3 classes:
* House: represents the layout of the house with walls and corridors, and maintains the cleaning state of the house (how much dirt per location).
* VaccumCleaner: Represents the vacuum cleaner and its functionality- clean, charge or move in a direction. In addition, maintaining the state of the vacuum cleaner along the program: current location, battery level etc.
* CleaningAlgorithm: Contains logic for determining the moves of the vacuum cleaner, conveying its decisions with getNextMove method. The class preserves moving and charging states between calls, so it can maintain logic based on the previous steps and current state.

In addition, in index.cpp we have the files I/O functionality, as well as initializing all the objects and initiation of the cleaning process.
## Input Format
Here is the input file format the program expects:

a b c d

-row 1-

...

-row n-

Where <a,b> are positive indexes stating the docking station coordinates (starting from zero); c is a positive integer of max battery steps; d is a positive integer of max steps.
The rows represent the rectangle house map, where there could be any value from -1 to 9. -1 represents a wall, 0-9 indicates dirt level.
The docking station coordinates on the map must be with value 0 (no dirt or wall there).

## Output Format
In the output format you can find:
* Number of steps made
* Total dirt left
* Battery state at the end of the run
* Print of the house map state at the end of the run
* An indication if the clean was successful
* A detailed log of all the steps made
