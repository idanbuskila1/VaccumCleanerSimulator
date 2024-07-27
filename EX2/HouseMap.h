#ifndef HOUSEMAP_H
#define HOUSEMAP_H

#include <set>
#include <map>
#include <stack>
#include <utility>  // for std::pair
#include "enums.hpp"
#include <vector>
#include <queue>

using std::vector,std::pair,std::queue,std::map;

class HouseMap {
    std::map<std::pair<int, int>, int> visited;    // Cells the vacuum has actively been on
    std::set<std::pair<int, int>> needToVisit;  // Cells the vacuum has seen but not visited, no walls
    size_t totalDirt = 0;
    size_t maxBattery = 0;
public:
    // Constructor
    HouseMap() = default;
    // Function to set the max battery level. needed to know if the vacuum cleaner can reach a certain cell
    void setMaxBattery(size_t max){maxBattery = max;};
    // Function to mark a cell as visited with a certain dirt level
    void markVisited(int x, int y, int dirt);
    // Function to mark a cell as seen but not visited
    void markToVisit(int x, int y);
    // Function to check if a cell is visited
    bool isVisited(int x, int y) const;
    // Function to check if a cell is needed to visit
    bool isInNeedToVisit(int x, int y) const;
    // Function to get the dirt level of a cell
    int getDirt(int x, int y) const;
    // Function to set the dirt level of a cell as newDirt
    int setDirt(int x, int y,int newDirt);
    // Function to get the total dirt level in the visited tiles
    int getTotalDirt() const{return totalDirt;};
    //is needToVisit empty
    bool isNeedToVisitEmpty() const{return needToVisit.empty();};
    // Function to check if a cell is reachable given maxBattery
    bool isReachable(pair<int,int> dest)const;
    // Function to add a cell to visited
    void addToVisited(int x, int y, int dirt){visited[{x,y}] = dirt;};
    // Function to erase a cell from needToVisit
    void eraseFromNeedToVisit(int x, int y){needToVisit.erase({x,y});};
    //when explore=false returns a shortest path between start and end. when explore=true returns a path to a visited cell with left dirt/a cell in needToVisit/empty path if there is none of the above.
    std::stack<Step> getShortestPath(const pair<int,int> start,const  pair<int,int> end, bool explore=false) const;

};

#endif // HOUSEMAP_H
