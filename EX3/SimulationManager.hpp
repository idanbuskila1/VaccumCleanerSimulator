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
vector<InputFileData*> houseFiles;
std::atomic_int simulationNo=0;

string processHouseFile(const string& path);
bool isInteger(const string& str);
string trim(const string& str) ;

public:
    void initializeHouses(string path);
    void operateSimulations();
};

#endif