#ifndef INDEX_H
#define INDEX_H
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <tuple>
#include <fstream>
#include <sstream>
#include "CleaningAlgorithm.h"
#include "House.h"
#include "VaccumCleaner.h"

using std::vector, std::tuple, std::string,  std::stack, std::max,  std::min,std::get;
using std::ostringstream, std::to_string, std::ifstream,std::ofstream, std::istringstream;
using std::cerr, std::endl, std::cout;

enum {WALL=-1,DIRT0,DIRT1,DIRT2,DIRT3,DIRT4,DIRT5,DIRT6,DIRT7,DIRT8,DIRT9};
enum{NORTH, EAST, SOUTH, WEST,CLEAN, CHARGE, STAY=4};

int updateDataFromFile(const string fileName, tuple<int,int> &dockingStationLoc,vector<vector<int>> &houseMap,int& maxBatterySteps,int& maxSteps);//TODO:update all data parameters from file.
int makeOutputFile(const VaccumCleaner& vc,const  House& h, const vector<string>* stepLog);
#endif