#ifndef HOUSEMAP_H
#define HOUSEMAP_H

#include <set>
#include <map>
#include <stack>
#include <utility>  // for std::pair
#include "utils.h"


class HouseMap {
    std::map<std::pair<int, int>, int> visited;    // Cells the vacuum has actively been on
    std::set<std::pair<int, int>> needToVisit;  // Cells the vacuum has seen but not visited, no walls
    size_t totalDirt = 0;
    size_t maxBattery = 0;
public:
    // Constructor
    HouseMap() = default;
    void setMaxBattery(size_t max){maxBattery = max;};
    // Function to mark a cell as visited with a certain dirt level
    void markVisited(int x, int y, int dirt);
    // Function to mark a cell as seen but not visited
    void markToVisit(int x, int y);
    // Function to check if a cell is visited
    bool isVisited(int x, int y) const;
    // Function to check if a cell is unvisited
    bool isInNeedToVisit(int x, int y) const;
    // Function to get the dirt level of a cell
    int getDirt(int x, int y) const;
    // Function to get the dirt level of a cell
    int setDirt(int x, int y);
    int getTotalDirt() const{return totalDirt;};
    std::stack<Step> getShortestPath(const pair<int,int> start,const  pair<int,int> end, bool explore=false) const;
    std::pair<int, int> findNotVisited() const;
    bool isNeedToVisitEmpty() const{return needToVisit.empty();};
    bool isReachable(pair<int,int> dest)const;
    void addToVisited(int x, int y, int dirt){visited[{x,y}] = dirt;};
    void eraseFromNeedToVisit(int x, int y){needToVisit.erase({x,y});};

};

#endif // HOUSEMAP_H
