#ifndef HOUSEMAP_H
#define HOUSEMAP_H

#include <set>
#include <map>
#include <utility>  // for std::pair
#include "utils.h"

class HouseMap {
    std::map<std::pair<int, int>, int> visited;    // Cells the vacuum has actively been on
    std::set<std::pair<int, int>> mappedHouse;  // Cells the vacuum has seen but not visited, no walls
public:
    // Constructor
    HouseMap() = default;
    // Function to mark a cell as visited with a certain dirt level
    void markVisited(int x, int y, int dirt);
    // Function to mark a cell as seen but not visited
    void markSeen(int x, int y);
    // Function to check if a cell is visited
    bool isVisited(int x, int y) const;
    // Function to check if a cell is unvisited
    bool isMapped(int x, int y) const;
    // Function to get the dirt level of a cell
    int getCurrDirt(int x, int y) const;
    vector<Step> getShortestPath(const pair<int,int> start,const  pair<int,int> end) const;
    std::map<std::pair<int, int>, int> getVisited() const { return visited; }
    std::set<std::pair<int, int>> getMappedHouse() const { return mappedHouse; }
    std::pair<int, int> findNotVisited() const;

};

#endif // HOUSEMAP_H
