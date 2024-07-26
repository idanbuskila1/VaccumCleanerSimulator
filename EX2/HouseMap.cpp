#include "HouseMap.h"

// Function to mark a cell as visited-we only put in the cells that aren't walls
void HouseMap::markVisited(int x, int y, int dirt) {
    std::pair<int, int> cell = std::make_pair(x, y);
    visited[cell] = dirt; //TODO check syntax
    totalDirt += dirt;
}

// Function to mark a cell as seen but not visited
void HouseMap::markToVisit(int x, int y) {
    std::pair<int, int> cell = std::make_pair(x, y);
    needToVisit.insert(cell);
}

// Function to check if a cell is visited
bool HouseMap::isVisited(int x, int y) const {
    return visited.find(std::make_pair(x, y)) != visited.end();
}

// Function to check if a cell is seen
bool HouseMap::isInNeedToVisit(int x, int y) const {
    return needToVisit.find(std::make_pair(x, y)) != needToVisit.end();
}

// Function to get the dirt level of a cell
int HouseMap::getDirt(int x, int y) const {
    if(needToVisit.find(std::make_pair(x, y)) != needToVisit.end()){
        return visited.at(std::make_pair(x, y));
    }
    return -1;
}
int HouseMap::setDirt(int x, int y) {
    if(needToVisit.find(std::make_pair(x, y)) != needToVisit.end()){
        visited[std::make_pair(x, y)] -=1;
        totalDirt -=1;
    }
    return -1;
}
bool HouseMap::isReachable(pair<int,int> dest)const{
    auto path = getShortestPath({0,0}, dest);
    if(path.size()*2 >= maxBattery){
        return false;
    }
    return true;
}
queue<Step> HouseMap::getShortestPath(const pair<int,int> start, const pair<int,int> end, bool explore) const {
    queue<Step> directions;
    if (!explore && start == end) return directions;

    vector<std::pair<int, int>> moves = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    vector<Step> moveDirections = {Step::North, Step::East, Step::South, Step::West};

    queue<pair<int, int>> q;
    map<pair<int, int>, pair<int, int>> cameFrom;
    map<pair<int, int>, Step> moveTaken;

    q.push(start);
    cameFrom[start] = start;

    while (!q.empty()) {
        pair<int, int> current = q.front();
        q.pop();

        for (size_t i = 0; i < moves.size(); ++i) {
            pair<int, int> neighbor = {current.first + moves[i].first, current.second + moves[i].second};

            if ((needToVisit.find(neighbor) != needToVisit.end() || visited.count(neighbor) != 0 )&& cameFrom.find(neighbor) == cameFrom.end()) {
                q.push(neighbor);
                cameFrom[neighbor] = current;
                moveTaken[neighbor] = moveDirections[i];

                if(explore){
                    if(isReachable(neighbor)&&((visited.count(neighbor)!=0 && visited.at(neighbor)>0) || needToVisit.find(neighbor) != needToVisit.end())){
                        std::queue<Step> path;
                        pair<int, int> loc = neighbor;
                         while (loc != start) {
                            path.push(moveTaken[loc]);
                            loc = cameFrom[loc];
                        }
                        return path;
                    }
                }
                else if (neighbor == end) {
                    std::queue<Step> path;
                    pair<int, int> loc = end;
                    while (loc != start) {
                        path.push(moveTaken[loc]);
                        loc = cameFrom[loc];
                    }
                    return path;
                }
            }
        }
    }
    return directions;
}

pair<int, int> HouseMap::findNotVisited() const { 
    for (const auto& m : needToVisit) {
        if (!isVisited(m.first, m.second)) {
            return m;
        }
    }
    return pair<int,int>{-1,-1};
}