#ifndef INDEX_H
#define INDEX_H
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <tuple>
#include "House.h"
#include "VaccumCleaner.h"
#include <fstream>
#include <sstream>

enum {WALL=-1,DIRT0,DIRT1,DIRT2,DIRT3,DIRT4,DIRT5,DIRT6,DIRT7,DIRT8,DIRT9};
enum{NORTH, EAST, SOUTH, WEST,CLEAN, CHARGE};

int chooseNextDirection(House h, std::tuple<int,int> loc);//choose next direction to go from loc.
void updateDataFromFile(std::string fileName,const std::tuple<int,int> &dockingStationLoc,std::vector<std::vector<int>> &houseMap,int* maxBatterySteps,int* MaxSteps);//TODO:update all data parameters from file.
//void makeOutputFile(VaccumCleaner& vc, House& h);//TODO: make output file as instructed in the PDF.
#endif