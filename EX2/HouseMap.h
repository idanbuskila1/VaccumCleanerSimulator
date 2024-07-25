#ifndef HOUSEMAP_H
#define HOUSEMAP_H

#include <set>
#include <map>
#include <utility>  // for std::pair
#include "utils.h"

class HouseMap {
public:
    // Constructor
    HouseMap() = default;
    HouseMap(pair<int, int> dockingLocation) : dockingLocation(dockingLocation) {}
    // Function to set the docking location
    void setDockingLocation(pair<int, int> dockingLocation) { this->dockingLocation = dockingLocation; }
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
    vector<Direction> getShortestPath(pair<int,int> start, pair<int,int> end) const;
    std::map<std::pair<int, int>, int> getVisited() const { return visited; }
    std::set<std::pair<int, int>> getMappedHouse() const { return mappedHouse; }
    std::pair<int, int> findNotVisited() const;

private:
    std::map<std::pair<int, int>, int> visited;    // Cells the vacuum has actively been on
    std::set<std::pair<int, int>> mappedHouse;  // Cells the vacuum has seen but not visited, no walls
    std::pair<int, int> dockingLocation;
};

#endif // HOUSEMAP_H
