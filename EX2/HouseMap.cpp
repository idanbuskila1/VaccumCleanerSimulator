#include "HouseMap.h"

// Function to mark a cell as visited-we only put in the cells that aren't walls
void HouseMap::markVisited(int x, int y, int dirt) {
    std::pair<int, int> cell = std::make_pair(x, y);
    visited[cell] = dirt; //TODO check syntax
}

// Function to mark a cell as seen but not visited
void HouseMap::markSeen(int x, int y) {
    std::pair<int, int> cell = std::make_pair(x, y);
    mappedHouse.insert(cell);
}

// Function to check if a cell is visited
bool HouseMap::isVisited(int x, int y) const {
    return visited.find(std::make_pair(x, y)) != visited.end();
}

// Function to check if a cell is seen
bool HouseMap::isMapped(int x, int y) const {
    return mappedHouse.find(std::make_pair(x, y)) != mappedHouse.end();
}

// Function to get the dirt level of a cell
int HouseMap::getCurrDirt(int x, int y) const {
     try {
        // Attempt to access the value associated with the key
        return visited.at(std::make_pair(x, y));
    } catch (const std::out_of_range& e) {
        // Key not found, return -999
        return -999;
    }
}

vector<Direction> HouseMap::getShortestPath(pair<int,int> start, pair<int,int> end) const {
    vector<Direction> directions;
    if (start == end) return directions;

    vector<std::pair<int, int>> moves = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    vector<Direction> moveDirections = {Direction::North, Direction::East, Direction::South, Direction::West};

    queue<pair<int, int>> q;
    map<pair<int, int>, pair<int, int>> cameFrom;
    map<pair<int, int>, Direction> moveTaken;

    q.push(start);
    cameFrom[start] = start;

    while (!q.empty()) {
        pair<int, int> current = q.front();
        q.pop();

        for (size_t i = 0; i < moves.size(); ++i) {
            pair<int, int> neighbor = {current.first + moves[i].first, current.second + moves[i].second};

            if (mappedHouse.find(neighbor) != mappedHouse.end() && cameFrom.find(neighbor) == cameFrom.end()) {
                q.push(neighbor);
                cameFrom[neighbor] = current;
                moveTaken[neighbor] = moveDirections[i];

                if (neighbor == end) {
                    vector<Direction> path;
                    pair<int, int> Direction = end;
                    while (Direction != start) {
                        path.push_back(moveTaken[Direction]);
                        Direction = cameFrom[Direction];
                    }
                    std::reverse(path.begin(), path.end());
                    return path;
                }
            }
        }
    }

    return directions;
}

pair<int, int> HouseMap::findNotVisited() const { 
    for (const auto& m : mappedHouse) {
        if (!isVisited(m.first, m.second)) {
            return m;
        }
    }
    return pair<int,int>{-1,-1};
}