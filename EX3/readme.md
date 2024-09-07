# Advanced Topics In Programming: Assignment 3
## contributor:
* Idan Buskila 208150953
I have worked with Ido Beerie 315140830 on Assignments 1 and 2 during this semester. we have worked completely sapparately on this last assignment, but there might be similiar code from the previous assignments - so know this is not due to copying.
## Folders Description
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
* the path to the .so files is "build/soFiles"
* the path to the house files is "houses"

