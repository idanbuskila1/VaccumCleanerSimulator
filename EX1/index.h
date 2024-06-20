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


enum {WALL=-1,DIRT0,DIRT1,DIRT2,DIRT3,DIRT4,DIRT5,DIRT6,DIRT7,DIRT8,DIRT9};
enum{NORTH, EAST, SOUTH, WEST,CLEAN, CHARGE, STAY=4};

void updateDataFromFile(std::string fileName,const std::tuple<int,int> &dockingStationLoc,std::vector<std::vector<int>> &houseMap,int* maxBatterySteps,int* MaxSteps);//TODO:update all data parameters from file.
void makeOutputFile(VaccumCleaner& vc, House& h, std::vector<std::string>* stepLog);
#endif