#include "HouseManager.h"

void HouseManager::markAsVisited(int x, int y, int dirt) {
    std::pair<int, int> cell = std::make_pair(x, y);
    cells_visited[cell] = dirt; //TODO check syntax
    totalDirt += dirt;
}

void HouseManager::setNeedToVisit(int x, int y) {
    std::pair<int, int> cell = std::make_pair(x, y);
    cells_to_visit.insert(cell);
}

bool HouseManager::isVisited(int x, int y) const {
    return cells_visited.find(std::make_pair(x, y)) != cells_visited.end();
}

bool HouseManager::isNeededVisit(int x, int y) const {
    bool ret = cells_to_visit.find(std::make_pair(x, y)) != cells_to_visit.end();
    return ret;
}


std::stack<Step> HouseManager::getShortestPath(const pair<int,int> start, const pair<int,int> end, bool shouldExplore) const {
    std::stack<Step> dirs;
    if (!shouldExplore && start == end) return dirs;

    vector<std::pair<int, int>> moves = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    vector<Step> moveDirs = {Step::North, Step::East, Step::South, Step::West};

    queue<pair<int, int>> q;
    map<pair<int, int>, pair<int, int>> sourceLoc;
    map<pair<int, int>, Step> stepMade;

    q.push(start);
    sourceLoc[start] = start;

    while (!q.empty()) {
        pair<int, int> current = q.front();
        q.pop();

        for (size_t i = 0; i < moves.size(); ++i) {
            pair<int, int> neighbor = {current.first + moves[i].first, current.second + moves[i].second};

            if ((cells_to_visit.find(neighbor) != cells_to_visit.end() || cells_visited.count(neighbor) != 0 )&& sourceLoc.find(neighbor) == sourceLoc.end()) {
                q.push(neighbor);
                sourceLoc[neighbor] = current;
                stepMade[neighbor] = moveDirs[i];

                if(shouldExplore){
                    if(isReachable(neighbor)&&((cells_visited.count(neighbor)!=0 && cells_visited.at(neighbor)>0) || cells_to_visit.find(neighbor) != cells_to_visit.end())){
                        std::stack<Step> path;
                        pair<int, int> loc = neighbor;
                         while (loc != start) {
                            path.push(stepMade[loc]);
                            loc = sourceLoc[loc];
                        }
                        return path;
                    }
                }
                else if (neighbor == end) {
                    std::stack<Step> path;
                    pair<int, int> loc = end;
                    while (loc != start) {
                        path.push(stepMade[loc]);
                        loc = sourceLoc[loc];
                    }
                    return path;
                }
            }
        }
    }
    return dirs;
}

bool HouseManager::isReachable(pair<int,int> dest)const{
    auto path = getShortestPath({0,0}, dest);
    if(path.size()*2 >= maxBattery){
        return false;
    }
    return true;
}

int HouseManager::getDirt(int x, int y) const {
    if(cells_to_visit.find(std::make_pair(x, y)) != cells_to_visit.end()){
        return cells_visited.at(std::make_pair(x, y));
    }
    return -1;
}

int HouseManager::setDirt(int x, int y,int newDirt) {
    if(cells_visited.find(std::make_pair(x, y)) != cells_visited.end()){
        int previousDirt = cells_visited.at(std::make_pair(x, y));
        cells_visited[std::make_pair(x, y)] =newDirt;
        totalDirt -=(previousDirt-newDirt);
    }
    return -1;
}

pair<int, int> HouseManager::findNotVisited() const { 
    for (const auto& m : cells_to_visit) {
        if (!isVisited(m.first, m.second)) {
            return m;
        }
    }
    return pair<int,int>{-1,-1};
}