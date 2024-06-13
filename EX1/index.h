#ifndef INDEX_H
#define INDEX_H
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <tuple>
#include "House.h"
#include "VaccumCleaner.h"
using namespace std;

enum {WALL=-1,DIRT0,DIRT1,DIRT2,DIRT3,DIRT4,DIRT5,DIRT6,DIRT7,DIRT8,DIRT9};
enum{NORTH, EAST, SOUTH, WEST,CLEAN, CHARGE};

int chooseNextDirection(House h, tuple<int,int> loc);//choose next direction to go from loc.
//void updateDataFromFile(string fileName,const tuple<int,int> &dockingStationLoc,vector<vector<int>> &houseMap,int* maxBatterySteps,int* MaxSteps);//TODO:update all data parameters from file.
//void makeOutputFile(VaccumCleaner& vc, House& h);//TODO: make output file as instructed in the PDF.
#endif