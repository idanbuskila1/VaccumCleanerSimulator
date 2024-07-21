#pragma once

#include "enums.hpp"

#include <map>
#include <stack>

using std::pair;

class HouseManager {
private:
  std::map<std::pair<int, int>, int> percieved_house_;
  std::map<std::pair<int, int>, bool> unexplored_points_;

  long long total_dirt_;

public:
  HouseManager();
  // @todo add const

  int total_dirt() const { return total_dirt_; }

  bool exists(const pair<int,int> pos) const;
  int dirt(const pair<int,int> pos);
  void setDirt(const pair<int,int> pos, int level);
  bool isWall(const pair<int,int> pos);
  bool isDock(const pair<int,int> pos);
  void clean(const pair<int,int> pos);
  // update and clean
  void clean(const pair<int,int> pos, int dirt);

  bool isUnexploredEmpty();
  bool isUnexplored(const pair<int,int> pos);
  // check and erase
  void eraseUnexplored(const pair<int,int> pos);

  // void updateNeighbors();
  void updateNeighbor(Direction dir, pair<int,int> position, bool isWall);

  std::stack<Direction> getShortestPath(std::pair<int, int> src,
                                        std::pair<int, int> dst,
                                        bool search = false);

  std::vector<std::pair<int, int>> neighbors(std::pair<int, int> point);
};
