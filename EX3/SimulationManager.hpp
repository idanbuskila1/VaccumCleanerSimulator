#ifndef SIMULATION_MANAGER
#define SIMULATION_MANAGER
#include "common/structs.hpp"
#include "simulator/Simulator.hpp"
#include "common/AlgorithmRegistrar.h"
#include <atomic>
#include <string>
#include <filesystem>
#include <iostream>

using std::string;

class SimulationManager{
vector<InputFileData> houseFiles;
std::atomic<int> simulationNo{0};
vector<vector<int>> scores;


string processHouseFile(const string& path, const string& strippedName);
bool isInteger(const string& str);
string trim(const string& str) ;

public:
    void initializeHouses(string path);
    void operateSimulations(bool isSummaryOnly);
    void makeSummary();
};

#endif