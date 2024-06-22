#ifndef HOUSE_H
#define HOUSE_H


class House{
    const tuple<int,int> houseSize;
    const tuple<int,int> dockingStationLoc; 
    int totalDirtLeft;
    vector<vector<int>> houseMap;//vector of vectors 

    bool isLocInsideHouse(const tuple<int,int>& loc) const;//returns true iff loc is valid with regard to houseSize
public:
    House(const tuple<int,int> dockingLoc,const vector<vector<int> > map);
    void updateCleaningState(const tuple<int,int> loc);//updates cleaning step on location loc, and decreases dirt level by 1.
    int getDirtLevel(const tuple<int,int> loc) const;//returns dirt level on loc, or 0 if there is no dirt/wall.
    bool isWallInDirection(const int direction, const tuple<int,int> curLoc) const;//according to direction and current location return if there is a wall.
    int getTotalDirtLeft()const{return totalDirtLeft;}//returns the sum of dirt left in the entire house.
    tuple<int,int> getDockingStationLoc()const{return dockingStationLoc;}//returns the location of the docking station.
    string toString() const;
};
#endif