#include "House.h"
House::House(const vector<vector<HouseState>> map):houseMap(map){
    int totalDirt=0;
    for(int i=0;i<map.size();i++){
        for(int j=0;j<map[i].size();j++){
            if(map[i][j]== HouseState::Docking)
                dockingStationLoc = make_pair(i,j);
            else if(map[i][j]==HouseState::Wall || map[i][j]==HouseState::Empty||map[i][j]==HouseState::Dirt0)
                continue;
            else totalDirt+=static_cast<int>(map[i][j]);
        }  
    }
    totalDirtLeft=totalDirt;
}
bool House::isLocInsideHouse(const int row, const int col) const{
    if(row>=houseMap.size()||col>=houseMap[row].size() ||row<0 || col<0  )
        return false;
    return true;
}
void House::updateCleaningState(const pair<int,int> loc){
    if(!isLocInsideHouse(loc.first,loc.second))
        return;//cur loc not inside the house so practicly clean. dont crash the program.
    HouseState oldState = houseMap[loc.first][loc.second];
    if(oldState== HouseState::Dirt0 || oldState== HouseState::Docking||oldState== HouseState::Empty||oldState== HouseState::Wall)//allready clean or a wall.
        return;//do anything.
    houseMap[loc.first][loc.second]=static_cast<HouseState>(static_cast<int>(oldState)-1);
    totalDirtLeft -=1;
}
bool House::isWallInDirection(const Direction direction, const pair<int,int> curLoc) const{
    if(direction == Direction::North){
        if(!isLocInsideHouse(curLoc.first-1,curLoc.second)) return true;
        return this->houseMap[curLoc.first-1][curLoc.second] == HouseState::Wall;
    }
    if(direction == Direction::South){
        if(!isLocInsideHouse(curLoc.first+1,curLoc.second)) return true;
        return this->houseMap[curLoc.first+1][curLoc.second] == HouseState::Wall;
    }
    if(direction == Direction::East){
        if(!isLocInsideHouse(curLoc.first,curLoc.second+1)) return true;
        return this->houseMap[curLoc.first][curLoc.second+1] == HouseState::Wall;
    }
    if(direction == Direction::West){
        if(!isLocInsideHouse(curLoc.first,curLoc.second-1)) return true;
        return this->houseMap[curLoc.first][curLoc.second-1] == HouseState::Wall;
    }
    return true;
}
int House::getDirtLevel(const pair<int,int> loc)const{
    if(!isLocInsideHouse(loc.first,loc.second))
        return 0;//dont crash the program. return there is no dirt if the index is outside the house
    HouseState state = houseMap[loc.first][loc.second];
    if(state== HouseState::Dirt0 || state== HouseState::Docking||state== HouseState::Empty||state== HouseState::Wall)//allready clean or a wall.
        return 0;
    return static_cast<int>(state);

};