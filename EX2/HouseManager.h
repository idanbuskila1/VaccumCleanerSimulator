#ifndef HOUSEMANAGER_H
#define HOUSEMANAGER_H

#include <set>
#include <map>
#include <stack>
#include <utility>
#include "utils.h"


class HouseManager {
    std::set<std::pair<int, int>> cells_to_visit;  // Cells the vacuum has not visited yet
    std::map<std::pair<int, int>, int> cells_visited;    // Cells the vacuum has visited already
    size_t totalDirt = 0;
    size_t maxBattery = 0;
public:
    // Constructor
    HouseManager() = default;
    void setNeedToVisit(int x, int y); // mark the loc as need to visit
    int getDirt(int x, int y) const; // get the dirt level in loc
    bool isReachable(pair<int,int> dest)const; // check if the destination is reachable with the current battery
    void setMaxBattery(size_t max){maxBattery = max;}; // set the max battery of the vaccum
    int setDirt(int x, int y,int newDirt);
    int getTotalDirt() const{return totalDirt;};
    bool isNeededVisit(int x, int y) const; // check if a loc is needed a visit
    std::stack<Step> getShortestPath(const pair<int,int> start,const  pair<int,int> end, bool explore=false) const; // get the shortest path from start to end
    std::pair<int, int> findNotVisited() const; // find the nearest not visited cell
    void markAsVisited(int x, int y, int dirt); // set the loc as visited
    bool isVisited(int x, int y) const; // check if the loc is visited
    bool isCellsToVisitEmpty() const{return cells_to_visit.empty();};
    void addToCellsVisited(int x, int y, int dirt){cells_visited[{x,y}] = dirt;};
    void removeFromNeedVisit(int x, int y){cells_to_visit.erase({x,y});};

};

#endif // HOUSEMANAGER_H
