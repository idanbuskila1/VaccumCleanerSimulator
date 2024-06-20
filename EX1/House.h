#ifndef HOUSE_H
#define HOUSE_H


class House{
    const std::tuple<int,int> houseSize;
    const std::tuple<int,int> dockingStationLoc; 
    int totalDirtLeft;
    std::vector<std::vector<int>> houseMap;//vector of vectors 

    bool isLocInsideHouse(const std::tuple<int,int>& loc) const;//returns true iff loc is valid with regard to houseSize
public:
    House(const std::tuple<int,int> dockingLoc,std::vector<std::vector<int> > map);
    void updateCleaningState(const std::tuple<int,int> loc);//updates cleaning step on location loc, and decreases dirt level by 1.
    int getDirtLevel(const std::tuple<int,int> loc) const;//returns dirt level on loc, or 0 if there is no dirt/wall.
    bool isWallInDirection(int direction, const std::tuple<int,int> curLoc) const;//according to direction and current location return if there is a wall.
    int getTotalDirtLeft()const{return totalDirtLeft;}//returns the sum of dirt left in the entire house.
    std::tuple<int,int> getDockingStationLoc()const{return dockingStationLoc;}//returns the location of the docking station.
    std::string toString() const;
};
#endif