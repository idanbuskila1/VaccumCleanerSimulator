#ifndef HOUSE
#define HOUSE

#include <vector>
#include <string>
#include <cstddef>
#include <memory>
#include <iostream>
#include "enums.h"

using  std::pair, std::make_pair,std::make_unique,std::vector, std::string, std::max,  std::min,std::get;
using std::cerr, std::endl, std::cout;

class House{
    pair<int,int> dockingStationLoc; 
    int totalDirtLeft;
    vector<vector<int>> houseMap;

    bool isLocInHouse(const size_t row, const size_t col) const; //true if loc is in house bounds.
public:
    House(const vector<vector<int> > map);

    void updateCleaningState(const pair<int,int> loc); //decrease dirt by 1 in loc and decrease totalDirtLeft by 1.
    int getDirtLevelInLoc(const pair<int,int> loc) const; //if loc is in house return the dirt eles 0.
    bool isWallInDirection(const Direction direction, const pair<int,int> curLoc) const; //true if there is a wall in the direction.
    int getTotalDirtLeft()const{return totalDirtLeft;} //total dirt left in the house.
    pair<int,int> getDockingStationLoc()const{return dockingStationLoc;} //locaiton of the vaccum docking station.
};
#endif