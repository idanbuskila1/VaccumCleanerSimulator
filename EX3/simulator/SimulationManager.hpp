#ifndef SIMULATION_MANAGER
#define SIMULATION_MANAGER
#include "../common/structs.hpp"
#include "Simulator.hpp"
#include "../common/AlgorithmRegistrar.h"
#include <atomic>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <dlfcn.h>
#include <dirent.h>
#include <thread>
using std::string;

class SimulationManager{
vector<InputFileData> houseFiles;
std::vector<void*> algorithmEntries;
std::atomic<int> simulationNo{0};
vector<vector<int>> scores;
bool isSummaryOnly;
bool isSimulationOver=false;

//read the house file in path, and return the house file data
string processHouseFile(const string& path, const string& strippedName);
//return true iff str is an integer
bool isInteger(const string& str);
//trim leading and trailing whitespaces from str
string trim(const string& str) ;
//return the algorithm with the given index
auto getAlgoByIndex(int idx);

public:
    //initialize the house files vector with the house files in path
    void initializeHouses(string path);
    //open all the algorithms in path
    void openAlgorithms(string path);
    //create an error file in errorFilePath with the given msg
    void createErrFile(string errorFilePath, string msg);
    //close all the algorithms and clear registrar
    void closeAlgorithms();
    //initialize simulators for all the simulation with the given house files and algorithms
    std::vector<Simulator> prepareAllSimulations();
    //summarize the simulations made and give scores
    void sumerrizeAllSimulations(std::vector<Simulator>& simulators,vector<int>& results);
    //return the time limit for the simulation
    size_t getTimeLimit(int simNo);
    //return the number of the next simulation to execute
    int getSimulationNumber();
    //determine if to make output files per simulation or not.
    void setIsSummaryOnly(bool flag){isSummaryOnly=flag;};
    //if all the simulations allready executed
    bool isSimulationDone(){return isSimulationOver;};
    //make the summary.csv file
    void makeSummary();
};

#endif