#include "HouseMap.h"

vector<std::pair<int, int>> HouseMap::directionsToMoves(vector<Step> directionPriorities) const{
        vector<std::pair<int, int>> moves;
        for(auto dir:directionPriorities){
            switch (dir)
            {
            case Step::North:
                moves.push_back({-1,0});
                break;
            case Step::East:
                moves.push_back({0,1});
                break;
            case Step::South:
                moves.push_back({1,0});
                break;
            case Step::West:
                moves.push_back({0,-1});
                break;
            default:
                break;
            }
        }
        return moves;
    }
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
    if(!isReachable(cell)){//if not reachable we erase it
        eraseFromNeedToVisit(x,y);
    }
}

// Function to check if a cell is visited
bool HouseMap::isVisited(int x, int y) const {
    return visited.find(std::make_pair(x, y)) != visited.end();
}

// Function to check if a cell is seen
bool HouseMap::isInNeedToVisit(int x, int y) const {
    bool ret = needToVisit.find(std::make_pair(x, y)) != needToVisit.end();
    return ret;
}

// Function to get the dirt level of a cell
int HouseMap::getDirt(int x, int y) const {
    if(needToVisit.find(std::make_pair(x, y)) != needToVisit.end()){
        return visited.at(std::make_pair(x, y));
    }
    return -1;
}
int HouseMap::setDirt(int x, int y,int newDirt) {
    if(visited.find(std::make_pair(x, y)) != visited.end()){
        int oldDirt = visited.at(std::make_pair(x, y));
        visited[std::make_pair(x, y)] =newDirt;
        totalDirt -=(oldDirt-newDirt);
    }
    return -1;
}
bool HouseMap::isReachable(pair<int,int> dest)const{
    auto path = getShortestPath({0,0}, dest,{Step::North, Step::East, Step::South, Step::West});
    if(path.size()*2 >= maxBattery){
        return false;
    }
    return true;
}
std::stack<Step> HouseMap::getShortestPath(const pair<int,int> start, const pair<int,int> end,vector<Step> directionPriorities, bool explore) const {
    vector<std::pair<int, int>> moves = directionsToMoves(directionPriorities);
    std::stack<Step> directions;
    if (!explore && start == end) return directions;

    //vector<std::pair<int, int>> moves = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    //vector<Step> directionPriorities = {Step::North, Step::East, Step::South, Step::West};
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
                moveTaken[neighbor] = directionPriorities[i];

                if(explore){
                    if(isReachable(neighbor)&&((visited.count(neighbor)!=0 && visited.at(neighbor)>0) || needToVisit.find(neighbor) != needToVisit.end())){
                        std::stack<Step> path;
                        pair<int, int> loc = neighbor;
                         while (loc != start) {
                            path.push(moveTaken[loc]);
                            loc = cameFrom[loc];
                        }
                        return path;
                    }
                }
                else if (neighbor == end) {
                    std::stack<Step> path;
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