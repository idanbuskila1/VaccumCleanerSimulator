#include "House.hpp"
House::House(const vector<vector<int>> map):houseMap(map){
  houseMap.resize(map.size() + 2);
  for (int i = 0; i < int(houseMap.size()); i++)
    houseMap[i].resize(map[0].size() + 2, 0);

  // Pad with wall
  for (int i = 0; i < int(houseMap[0].size()); i++) {
    houseMap[0][i] = int(LocType::Wall);
    houseMap[houseMap.size() - 1][i] = int(LocType::Wall);
  }

  for (int i = 0; i < int(houseMap.size()); i++) {
    houseMap[i][0] = int(LocType::Wall);
    houseMap[i][houseMap[0].size() - 1] = int(LocType::Wall);
  }

  for (int i = 0; i < int(map.size()); i++) {
    for (int j = 0; j < int(map[0].size()); j++) {
      if (map[i][j] == 100) {
        dockingStationLoc = {i + 1, j + 1};
        houseMap[i + 1][j + 1] = 0;
      } else {
        if (map[i][j] > 0)
          totalDirtLeft += map[i][j];
        houseMap[i + 1][j + 1] = map[i][j];
      }
    }
  }
}

void House::init(const vector<vector<int>> map){
  houseMap.resize(map.size() + 2);
  for (int i = 0; i <int( houseMap.size()); i++)
    houseMap[i].resize(map[0].size() + 2, 0);

  // Pad with wall
  for (int i = 0; i < int(houseMap[0].size()); i++) {
    houseMap[0][i] = int(LocType::Wall);
    houseMap[houseMap.size() - 1][i] = int(LocType::Wall);
  }

  for (int i = 0; i <int(houseMap.size()); i++) {
    houseMap[i][0] = int(LocType::Wall);
    houseMap[i][houseMap[0].size() - 1] = int(LocType::Wall);
  }

  for (int i = 0; i < int(map.size()); i++) {
    for (int j = 0; j < int(map[0].size()); j++) {
      if (map[i][j] == 100) {
        dockingStationLoc = {i + 1, j + 1};
        houseMap[i + 1][j + 1] = 0;
      } else {
        if (map[i][j] > 0)
          totalDirtLeft += map[i][j];
        houseMap[i + 1][j + 1] = map[i][j];
      }
    }
  }
}
bool House::isLocInsideHouse(const size_t row, const size_t col) const{
    if(row>=houseMap.size()||col>=houseMap[row].size() ||row<0 || col<0  )
        return false;
    return true;
}
void House::updateCleaningState(const pair<int,int> loc){
    if(!isLocInsideHouse(loc.first,loc.second))
        return;//cur loc not inside the house so practicly clean. dont crash the program.
    int oldState = houseMap[loc.first][loc.second];
    if(oldState== 0 || oldState== -2 ||oldState== -1)//allready clean or a wall or a docking.
        return;//do anything.
    houseMap[loc.first][loc.second]-=1;
    totalDirtLeft -=1;
}
bool House::isWallInDirection(const Direction direction, const pair<int,int> curLoc) const{
    // #print the curLoc
    std::cerr << "curLoc: " << curLoc.first << " " << curLoc.second << std::endl;
    if(direction == Direction::North){
        if(!isLocInsideHouse(curLoc.first-1,curLoc.second)) return true;
        return this->houseMap[curLoc.first-1][curLoc.second] == -1;
    }
    if(direction == Direction::South){
        if(!isLocInsideHouse(curLoc.first+1,curLoc.second)) return true;
        return this->houseMap[curLoc.first+1][curLoc.second] == -1;
    }
    if(direction == Direction::East){
        if(!isLocInsideHouse(curLoc.first,curLoc.second+1)) return true;
        return this->houseMap[curLoc.first][curLoc.second+1] == -1;
    }
    if(direction == Direction::West){
        if(!isLocInsideHouse(curLoc.first,curLoc.second-1)) return true;
        return this->houseMap[curLoc.first][curLoc.second-1] == -1;
    }
    return true;
}
bool House::isWall(const pair<int,int> curLoc) const {
  return this->houseMap[curLoc.first][curLoc.second] == int(LocType::Wall);
}

int House::getDirtLevel(const pair<int,int> loc)const{
    if(!isLocInsideHouse(loc.first,loc.second))
        return 0;//dont crash the program. return there is no dirt if the index is outside the house
    int state = houseMap[loc.first][loc.second];
    if(state== 0 || state== -2||state== -1)//allready clean or a wall or a docking.
        return 0;
    return state;

};