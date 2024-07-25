#ifndef HOUSE
#define HOUSE

#include <vector>
#include <string>
#include <cstddef>
#include <memory>
#include <iostream>
#include "enums.hpp"

using  std::pair, std::make_pair,std::make_unique,std::vector, std::string, std::max,  std::min,std::get;
using std::cerr, std::endl, std::cout;

class House{
    pair<int,int> dockingStationLoc; 
    int totalDirtLeft;
    vector<vector<int>> houseMap;//vector of vectors 

    bool isLocInsideHouse(const size_t row, const size_t col) const;//returns true iff loc is valid with regard to houseSize
public:
    House(const vector<vector<int> > map);

    void updateCleaningState(const pair<int,int> loc);//updates cleaning Direction on location loc, and decreases dirt level by 1.
    int getDirtLevel(const pair<int,int> loc) const;//returns dirt level on loc, or 0 if there is no dirt/wall.
    bool isWallInDirection(const Direction direction, const pair<int,int> curLoc) const;//according to direction and current location return if there is a wall.
    int getTotalDirtLeft()const{return totalDirtLeft;}//returns the sum of dirt left in the entire house.
    pair<int,int> getDockingStationLoc()const{return dockingStationLoc;}//returns the location of the docking station.
};
#endif