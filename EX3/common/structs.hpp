#ifndef STUCTS
#define STUCTS

#include <stddef.h>
#include <vector>
#include <utility>
#include <string>

using std::vector, std::pair;
struct InputFileData{
    std::string houseName;
    size_t maxBattery;
    size_t maxSteps;
    vector<vector<int>> grid;
    pair<int, int> dockingStation;
    
    InputFileData(std::string name, size_t battery, size_t steps, vector<vector<int>> g,pair<int, int> docking):houseName(name),maxBattery(battery),maxSteps(steps),grid(g),dockingStation(docking){};
};

#endif