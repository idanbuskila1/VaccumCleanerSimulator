#ifndef HOUSE_H
#define HOUSE_H
#include "index.h"
using namespace std;

class House{
    const tuple<int,int> houseSize;
    const tuple<int,int> dockingStationLoc; 
    int totalDirtLeft;
    vector<vector<int>> houseMap;//vector of vectors 

public:
    House(const tuple<int,int> dockingLoc,vector<vector<int> > map);
    void updateCleaningState(tuple<int,int> loc);//updates cleaning step on location loc, and decreases dirt level by 1.
    int getDirtLevel(tuple<int,int> loc);//returns dirt level on loc, or 0 if there is no dirt/wall.
    bool isWallInDirection(int direction, tuple<int,int> curLoc);//according to direction and current location return if there is a wall.
    int getTotalDirtLeft(){return totalDirtLeft;}//returns the sum of dirt left in the entire house.
    tuple<int,int> getDockingStationLoc(){return dockingStationLoc;}//returns the location of the docking station.
};
#endif