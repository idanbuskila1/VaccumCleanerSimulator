# Vaccum Cleaner Simulator
## Overview
The Vacuum Cleaner Simulator is a project designed to simulate the operation of a vacuum cleaner robot within a predefined environment of a house. The simulator allows for the testing and evaluation of different algorithms that control the robot's movement and cleaning efficiency.
## Project Structure
The project consists of the following main components:
* **Simulator**: The main executable that runs the simulation.
* **Algorithms**: Shared object files (.so) that contain different algorithms for controlling the vacuum cleaner robot.
* **Common**: Shared utilities and classes used by both the simulator and the algorithms.
## Purpose
The purpose of the Vacuum Cleaner Simulator is to provide a platform for testing and comparing different algorithms that control a vacuum cleaner robot on different house structures. The simulator evaluates the performance of these algorithms based on various metrics such as cleaning efficiency, time taken, and energy consumption.
## Architecture
### Simulator
The simulator is a binary executable (myrobot) that orchestrates the entire simulation process. It loads the algorithm shared object files,all the .house files, and initializes simulation environment for each house-algorithm pair, and manages its execution.
### Algorithms
The algorithms are implemented as shared object files (.so). Each algorithm defines a specific strategy for controlling the vacuum cleaner robot. The simulator dynamically loads these shared object files at runtime and uses them to control the robot's behavior.
### Classes Hierarchy 
The project follows a well-defined class hierarchy to ensure modularity and reusability. The main classes are:
* **SimulationManager**: Manages the overall simulation, including loading algorithms, initializing the environment, and collecting results.
* **Simulator:** Represents a simulation environment for a certain house-algorithm pair. Uses the House and VaccumCleaner classes which maintain the house and robot state during the simulation, and interacts with the algorithms to control the robot's movements.
* **Algorithm:** An abstract base class that defines the interface for all algorithms. Each algorithm must implement the methods defined in this class.
* **Task:** Represents a unit of work in the simulation. Each task runs an instance of the simulation with a specific algorithm and environment configuration.
### Threading and Timeouts
The simulator uses multithreading to run multiple simulations concurrently. This is achieved using the Boost.Asio library for asynchronous operations and thread management.
* **Thread Pool:** A pool of threads is created to run multiple simulations in parallel. Each thread runs a separate instance of the simulation.
* **Timeouts:** Each simulation task has a timeout to ensure that it does not run indefinitely. The Boost.Asio library is used to set timers and handle timeouts. If a task exceeds its allocated time, it is terminated, and the next task is started.
### Folders Description
* 208150953_A, 208150953_B: the folders of the 2 algorithm projects.
* algorithm: contains AlgorithmRegistration.h
* common: folder of files that are common to both simulator project and algorithms.
* houses: folder with all the .house files
* outputs: all the output files of house-algorithm simulations.
* logs: more descriptive log for every house-algorithm simulation.
* Simulator: files of the simulator project.
## Build Description
### build only algo projects:
* make alg1
* make alg2
### build only simulator project:
* make sim
### build all three projects:
* make compile
### build all projects and run:
* make compile-run
### run simulator project(without re-compiling):
* make run
  
## Running Description:
* The run command is in the makefile, under the target "run".
you can change the line there with the arguments you want to send, and after done editing - run the command "make run" as mentioned.
### Command Line Arguments
The simulator accepts several command line arguments to customize its behavior. Below are the descriptions of the different command line arguments:
* **-house_path=<path>**: Specifies the path to the directory containing the house files. Each house file represents a different environment for the vacuum cleaner robot to navigate and clean. if not provided, it looks for the files in the cwd.
* **-algo_path=<path>**: Specifies the path to the directory containing the algorithm shared object files (.so). These files define the different strategies for controlling the vacuum cleaner robot.if not provided, it looks for the files in the cwd.
* **-summary_only**: When this flag is set, the simulator will only output a csv file summary of al the the simulations results, without detailed logs and output files for every single house-algorithm pair.
* **-num_threads=<number>**:Specifies the number of threads to use for running the simulations. This allows for parallel execution of multiple simulations of house-algorithm pairs.
  
* the path to the .so files is "build/soFiles"
* the path to the house files is "houses"
## House File Format
The format of the house text file shall follow the following strict instructions:
* **Line 1:** house name / description - internal for the file, can be ignored by the simulation
* **Line 2:** MaxSteps for the simulation on this house, as: MaxSteps = <NUM>
* **Line 3:** MaxBattery (in steps) for the simulation on this house, as: MaxBattery = <NUM>
* **Line 4:** Number of Rows in house, as: Rows = <NUM>
* **Line 5:** Number of Cols in house, as: Cols = <NUM>
* **Lines 6** and on: the house itself, as described below
  
In case the file contains additional rows and/or cols beyond the required number as set in lines 4 and 5, the program shall ignore the redundant lines / cols.
The program should get an input file that represents a house, with walls, corridors, dirt levels and a single docking station. The following characters shall be used:
* Empty Space (“corridors”): space as well as any character which does not have other mapping below
* Wall: 'W'
* Dirt: Numbers from 0 to 9 should represent dirt level. Though 0 is valid and shall be supported, it is in fact the same as space.
* Docking Station: 'D'. There shall be one and only one docking station otherwise the file is invalid.

## Output Files Format
An output file shall be generated for each algorithm-ouse pair in the /outputs directory. file name shall be: <HouseName>-<AlgorithmName>.txt and it will be in the following format:

* **NumSteps = <NUMBER>**: Number of steps made by the robot.
* **DirtLeft = <NUMBER>**: How much dirt left after the robot finished or died.
* **Status = <FINISHED/WORKING/DEAD>**: Dead - dead battery and not on docking station. Finished - the robot cleaned all accessible points and returned to docking. Working - all other cases.
* **InDock = <TRUE/FALSE>** : if the robot ended on the docking.
* **Steps:** list of characters in one line, no spaces, from: NESWsF – the small s is for STAY, NESW are for North, East, South, West. F is for Finished - only if the algorithm actually returned “Finished”.
* **Score = <NUMBER>**: the grade of the algorithm on this given house, based on the following formula:
  * If DEAD => Score = MaxSteps + DirtLeft * 300 + 2000
  * Otherwise, If reported FINISHED and robot is NOT InDock => Score = MaxSteps + DirtLeft * 300 + 3000
  * Otherwise => Score = NumSteps + DirtLeft * 300 + (InDock? 0 : 1000).
  * lower score is better.
  
## Log Files
An additional log file will be generateed for each simulation, with elaborated information about each step.

## CSV Summary File
Will be generated for each run in the cwd, with rows for algorithms and columns for houses. the table content is the score for each pair.

## Conclusion
The Vacuum Cleaner Simulator provides a robust platform for testing and evaluating different algorithms for controlling a vacuum cleaner robot. Its modular architecture and use of multithreading ensure efficient and scalable simulations.
