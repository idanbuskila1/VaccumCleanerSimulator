#include "House.hpp"
House::House(const vector<vector<int>> map):houseMap(map){
    int totalDirt=0;
    for(size_t i=0;i<map.size();i++){
        for(size_t j=0;j<map[i].size();j++){
            if(map[i][j]== -2)//docking
                dockingStationLoc = make_pair(i,j);
            else if(map[i][j]==-1 || map[i][j]==0)//wall or no dirt
                continue;
            else totalDirt+=map[i][j];
        }  
    }
    totalDirtLeft=totalDirt;
}
bool House::isLocInHouse(const size_t row, const size_t col) const{
    if(row>=houseMap.size()||col>=houseMap[row].size() ||row<0 || col<0  )
        return false;
    return true;
}
void House::updateCleaningState(const pair<int,int> loc){
    if(!isLocInHouse(loc.first,loc.second))
        return; //loc not in house
    int oldState = houseMap[loc.first][loc.second];
    if(oldState== 0 || oldState== -2 ||oldState== -1)
        return;
    houseMap[loc.first][loc.second]-=1;
    totalDirtLeft -=1;
}
bool House::isWallInDirection(const Direction direction, const pair<int,int> curLoc) const{
    if(direction == Direction::North){
        if(!isLocInHouse(curLoc.first-1,curLoc.second)) return true;
        return this->houseMap[curLoc.first-1][curLoc.second] == -1;
    }
    if(direction == Direction::South){
        if(!isLocInHouse(curLoc.first+1,curLoc.second)) return true;
        return this->houseMap[curLoc.first+1][curLoc.second] == -1;
    }
    if(direction == Direction::East){
        if(!isLocInHouse(curLoc.first,curLoc.second+1)) return true;
        return this->houseMap[curLoc.first][curLoc.second+1] == -1;
    }
    if(direction == Direction::West){
        if(!isLocInHouse(curLoc.first,curLoc.second-1)) return true;
        return this->houseMap[curLoc.first][curLoc.second-1] == -1;
    }
    return true;
}

int House::getDirtLevelInLoc(const pair<int,int> loc)const{
    if(!isLocInHouse(loc.first,loc.second))
        return 0; //return 0 dirt if out of house 
    int state = houseMap[loc.first][loc.second];
    if(state== 0 || state== -2||state== -1)//loc is clean or docking station or wall
        return 0;
    return state;

};