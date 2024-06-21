#include "index.h"
House::House(const tuple<int,int> dockingLoc,vector<vector<int>> map):houseSize(map.size(),map[0].size()),dockingStationLoc(dockingLoc),houseMap(map){
    int totalDirt=0;
    for(int i=0;i<get<0>(houseSize);i++){
        for(int j=0;j<get<1>(houseSize);j++){
            if(map[i][j]>0)
                totalDirt+=map[i][j];
        }  
    }
    totalDirtLeft=totalDirt;
}
bool House::isLocInsideHouse(const tuple<int,int>& loc) const{
    if(get<0>(loc)>=get<0>(houseSize)||get<1>(loc)>=get<1>(houseSize) ||get<0>(loc)<0 || get<1>(loc)<0  )
        return false;
    return true;
}
void House::updateCleaningState(const tuple<int,int> loc){
    if(!isLocInsideHouse(loc))
        return;//cur loc not inside the house so practicly clean. dont crash the program.
    int oldState = houseMap[get<0>(loc)][get<1>(loc)];
    if(oldState<=0)//allready clean or a wall.
        return;
    this->houseMap[get<0>(loc)][get<1>(loc)]-=1;
    this->totalDirtLeft -=1;
}
bool House::isWallInDirection(const int direction, const tuple<int,int> curLoc) const{
    if(direction == NORTH && isLocInsideHouse({get<0>(curLoc)-1,get<1>(curLoc)})){
        return this->houseMap[get<0>(curLoc)-1][get<1>(curLoc)] == WALL;
    }
    if(direction == SOUTH && isLocInsideHouse({get<0>(curLoc)+1,get<1>(curLoc)})){
        return this->houseMap[get<0>(curLoc)+1][get<1>(curLoc)] == WALL;
    }
    if(direction == EAST && isLocInsideHouse({get<0>(curLoc),get<1>(curLoc)+1})){
        return this->houseMap[get<0>(curLoc)][get<1>(curLoc)+1] == WALL;
    }
    if(direction == WEST && isLocInsideHouse({get<0>(curLoc),get<1>(curLoc)-1})){
        return this->houseMap[get<0>(curLoc)][get<1>(curLoc)-1] == WALL;
    }
    return true;
}
int House::getDirtLevel(const tuple<int,int> loc)const{
    if(!isLocInsideHouse(loc))
        return 0;//dont crash the program. return there is no dirt if the index is outside the house
    return max(this->houseMap[get<0>(loc)][get<1>(loc)],0);//if there is WALL==-1 on loc, max will return 0, bacause there is no dirt there.

};
string House::toString() const{
    ostringstream ret;
     for(int i=0;i<get<0>(houseSize);i++){
        for(int j=0;j<get<1>(houseSize);j++){
            ret<<houseMap[i][j]<<" ";
        } 
        ret<<"\n"; 
    }
    ret<<"\n"; 
    return ret.str();
}