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


string processHouseFile(const string& path, const string& strippedName);
bool isInteger(const string& str);
string trim(const string& str) ;
auto getAlgoByIndex(int idx);

public:
    void initializeHouses(string path);
    void sumerrizeAllSimulations(std::vector<Simulator>& simulators,vector<int>& results);
    int getSimulationNumber();
    void openAlgorithms(string path);
    void createErrFile(string errorFilePath, string msg);
    void closeAlgorithms();
    std::vector<Simulator> prepareAllSimulations();
    size_t getTimeLimit(int simNo);
    void setIsSummaryOnly(bool flag){isSummaryOnly=flag;};
    bool isSimulationDone(){return isSimulationOver;};
    void makeSummary();
};

#endif