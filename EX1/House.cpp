#include "index.h"
House::House(const std::tuple<int,int> dockingLoc,std::vector<std::vector<int>> map):houseSize(map.size(),map[0].size()),dockingStationLoc(dockingLoc),houseMap(map){
    int totalDirt=0;
    for(int i=0;i<std::get<0>(houseSize);i++){
        for(int j=0;j<std::get<1>(houseSize);j++){
            if(map[i][j]>0)
                totalDirt+=map[i][j];
                //std::cout<< ":" << map[i][j]<<" ";
        }  
    }
    totalDirtLeft=totalDirt;
}
void House::updateCleaningState(const std::tuple<int,int> loc){
    int oldState = houseMap[std::get<0>(loc)][std::get<1>(loc)];
    if(oldState<=0)//allready clean or a wall.
        return;
    this->houseMap[std::get<0>(loc)][std::get<1>(loc)]-=1;
    this->totalDirtLeft -=1;
}
bool House::isWallInDirection(const int direction, const std::tuple<int,int> curLoc) const{
    if(direction == NORTH)
        return this->houseMap[std::get<0>(curLoc)+1][std::get<1>(curLoc)] == WALL;
    if(direction == SOUTH)
        return this->houseMap[std::get<0>(curLoc)-1][std::get<1>(curLoc)] == WALL;
    if(direction == EAST)
        return this->houseMap[std::get<0>(curLoc)][std::get<1>(curLoc)+1] == WALL;
    //else direction == WESTs
    return this->houseMap[std::get<0>(curLoc)][std::get<1>(curLoc)-1] == WALL;
}
int House::getDirtLevel(const std::tuple<int,int> loc)const{
    //cout<<"entered "<<get<0>(loc)<<","<<get<1>(loc)<<endl;
    if(std::get<0>(loc)<std::get<0>(this->houseSize)&&std::get<1>(loc)<std::get<1>(this->houseSize) &&std::get<0>(loc)>=0 && std::get<1>(loc)>=0  )
        return std::max(this->houseMap[std::get<0>(loc)][std::get<1>(loc)],0);//if there is WALL==-1 on loc, max will return 0, bacause there is no dirt there.
    else {
        //dont crash the program. return there is no dirt if the index is outside the house
        return 0;
    }
};