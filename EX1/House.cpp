#include "index.h"
House::House(const tuple<int,int> dockingLoc,vector<vector<int>> map):houseSize(tuple<int,int>(map.size(),map[0].size())),dockingStationLoc(dockingLoc),houseMap(map){
    int totalDirt=0;
    for(int i=0;i<get<0>(houseSize);i++){
        for(int j=0;j<get<1>(houseSize);j++){
            if(map[i][j]>0)
                totalDirt+=map[i][j];
                std::cout<< ":" << map[i][j]<<" ";
        }
    }
    totalDirtLeft=totalDirt;
}
void House::updateCleaningState(tuple<int,int> loc){
    int oldState = houseMap[get<0>(loc)][get<1>(loc)];
    if(oldState<=0)//allready clean or a wall.
        return;
    this->houseMap[get<0>(loc)][get<1>(loc)]-=1;
    this->totalDirtLeft -=1;
}
bool House::isWallInDirection(int direction, tuple<int,int> curLoc){
    if(direction == NORTH)
        return this->houseMap[get<0>(curLoc)+1][get<1>(curLoc)] == WALL;
    if(direction == SOUTH)
        return this->houseMap[get<0>(curLoc)-1][get<1>(curLoc)] == WALL;
    if(direction == EAST)
        return this->houseMap[get<0>(curLoc)][get<1>(curLoc)+1] == WALL;
    //else direction == WESTs
    return this->houseMap[get<0>(curLoc)][get<1>(curLoc)-1] == WALL;
}
int House::getDirtLevel(tuple<int,int> loc){
    //cout<<"entered "<<get<0>(loc)<<","<<get<1>(loc)<<endl;
    if(get<0>(loc)<get<0>(this->houseSize)&&get<1>(loc)<get<1>(this->houseSize) &&get<0>(loc)>=0 && get<1>(loc)>=0  )
        return max(this->houseMap[get<0>(loc)][get<1>(loc)],0);//if there is WALL==-1 on loc, max will return 0.
    else {
        //cout<<"wrong loc";
        return 0;
    }
};