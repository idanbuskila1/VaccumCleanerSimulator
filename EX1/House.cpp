#include "index.h"
House::House(const std::tuple<int,int> dockingLoc,std::vector<std::vector<int>> map):houseSize(map.size(),map[0].size()),dockingStationLoc(dockingLoc),houseMap(map){
    int totalDirt=0;
    for(int i=0;i<std::get<0>(houseSize);i++){
        for(int j=0;j<std::get<1>(houseSize);j++){
            if(map[i][j]>0)
                totalDirt+=map[i][j];
        }  
    }
    totalDirtLeft=totalDirt;
}
bool House::isLocInsideHouse(const std::tuple<int,int>& loc) const{
    if(std::get<0>(loc)>=std::get<0>(houseSize)||std::get<1>(loc)>=std::get<1>(houseSize) ||std::get<0>(loc)<0 || std::get<1>(loc)<0  )
        return false;
    return true;
}
void House::updateCleaningState(const std::tuple<int,int> loc){
    if(!isLocInsideHouse(loc))
        return;//cur loc not inside the house so practicly clean. dont crash the program.
    int oldState = houseMap[std::get<0>(loc)][std::get<1>(loc)];
    if(oldState<=0)//allready clean or a wall.
        return;
    this->houseMap[std::get<0>(loc)][std::get<1>(loc)]-=1;
    this->totalDirtLeft -=1;
}
bool House::isWallInDirection(const int direction, const std::tuple<int,int> curLoc) const{
    if(direction == NORTH && isLocInsideHouse({std::get<0>(curLoc)-1,std::get<1>(curLoc)})){
        return this->houseMap[std::get<0>(curLoc)-1][std::get<1>(curLoc)] == WALL;
    }
    if(direction == SOUTH && isLocInsideHouse({std::get<0>(curLoc)+1,std::get<1>(curLoc)})){
        return this->houseMap[std::get<0>(curLoc)+1][std::get<1>(curLoc)] == WALL;
    }
    if(direction == EAST && isLocInsideHouse({std::get<0>(curLoc),std::get<1>(curLoc)+1})){
        return this->houseMap[std::get<0>(curLoc)][std::get<1>(curLoc)+1] == WALL;
    }
    if(direction == WEST && isLocInsideHouse({std::get<0>(curLoc),std::get<1>(curLoc)-1})){
        return this->houseMap[std::get<0>(curLoc)][std::get<1>(curLoc)-1] == WALL;
    }
    return true;
}
int House::getDirtLevel(const std::tuple<int,int> loc)const{
    if(!isLocInsideHouse(loc))
        return 0;//dont crash the program. return there is no dirt if the index is outside the house
    return std::max(this->houseMap[std::get<0>(loc)][std::get<1>(loc)],0);//if there is WALL==-1 on loc, max will return 0, bacause there is no dirt there.

};
std::string House::toString() const{
    std::ostringstream ret;
     for(int i=0;i<std::get<0>(houseSize);i++){
        for(int j=0;j<std::get<1>(houseSize);j++){
            ret<<houseMap[i][j]<<" ";
        } 
        ret<<"\n"; 
    }
    ret<<"\n"; 
    return ret.str();
}