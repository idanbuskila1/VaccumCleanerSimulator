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
};

#endif