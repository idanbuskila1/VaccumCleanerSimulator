#include "MyAlgorithm.hpp"


MyAlgorithm::MyAlgorithm() {
    currentLocation = {0,0};
    pathStack.push({0,0});
    uniquePathStack.insert({0,0});
}

void MyAlgorithm::setBatteryMeter(const BatteryMeter& meter) {
    batteryMeter = &meter;
    maxBattery = batteryMeter->getBatteryState();
}

void MyAlgorithm::setWallsSensor(const WallsSensor& sensor) {
    wallsSensor = &sensor;
}

void MyAlgorithm::setDirtSensor(const DirtSensor& sensor) {
    dirtSensor = &sensor;
}

void MyAlgorithm::setMaxSteps(std::size_t max) {
    maxSteps = max;
}

void printSet(const std::set<std::pair<int, int>>& s) {
    for (const auto& p : s) {
        std::cout << "(" << p.first << ", " << p.second << ")\n";
    }
    std::cout << "------------------" << endl;
}

void printVector(const vector<Step>& vec) {
    for (const auto& row : vec) {
        std::cout << stepToString(row) << ' ';
    }
    cout << endl;
}

Step MyAlgorithm::nextStep() {
    
    /*if (steps >= 360) {
        cout << "Battery: " << batteryMeter->getBatteryState() << ", Counter steps: "<< steps << endl;
    }*/

    if (pathStack.empty()) { 
        std::cout << "pathstack empty" << endl;
        if (houseMap.getVisited().size() == houseMap.getMappedHouse().size()) {
            std::cout << "visited size: " << houseMap.getVisited().size() << " mapped size: " << houseMap.getMappedHouse().size() << endl;
            if (currentLocation == DOCK) {
                return Step::Finish;
            }
        }
    }

    if (steps >= maxSteps) {
        std::cout << "steps made: " << steps << " max steps: " << maxSteps << endl;
        return Step::Finish;
    }

    if (batteryMeter->getBatteryState() != maxBattery && currentLocation == DOCK) {
        return Step::Stay;
    } 
    //i check my neighbors and see who is a wall and add myself and my neighbors to the seen map
    houseMap.markSeen(currentLocation.first, currentLocation.second);
    if (!wallsSensor->isWall(Direction::North)) {
        houseMap.markSeen(currentLocation.first - 1, currentLocation.second);
    }
    if (!wallsSensor->isWall(Direction::East)) {
        houseMap.markSeen(currentLocation.first, currentLocation.second + 1);
    }
    if (!wallsSensor->isWall(Direction::South)) {
        houseMap.markSeen(currentLocation.first + 1, currentLocation.second);
    }
    if (!wallsSensor->isWall(Direction::West)) {
        houseMap.markSeen(currentLocation.first, currentLocation.second - 1);
    }
    int currDirt = dirtSensor->dirtLevel();
    if (currDirt > 0 && currDirt <= 9) {
        if (uniquePathStack.find(currentLocation) == uniquePathStack.end() && !houseMap.isVisited(currentLocation.first, currentLocation.second)) {
            pathStack.push(currentLocation);
            uniquePathStack.insert(currentLocation);
        }
    }

    //if i need to start returning to the docking station
    std::size_t currBattery = batteryMeter->getBatteryState();
    vector<Step> pathToDocking = houseMap.getShortestPath(currentLocation, DOCK);
    size_t remainingSteps = maxSteps - steps;
    if (currBattery == pathToDocking.size() || remainingSteps == pathToDocking.size())  {
        return pathToDocking[0];
    }

    //we still have dirt in the current cell
    if (currDirt > 0 && currDirt <= 9){
        return Step::Stay;
    }

    //remove myself from the stack and add myself to the visited map
    // if (!houseMap.isVisited(currentLocation.first, currentLocation.second)) {
    houseMap.markVisited(currentLocation.first, currentLocation.second, currDirt);
    if (pathStack.top() == currentLocation) {
        pathStack.pop();
        uniquePathStack.erase(currentLocation);
    }
    //add my neighbors to the stack
    if (!wallsSensor->isWall(Direction::North) && !houseMap.isVisited(currentLocation.first - 1, currentLocation.second) && uniquePathStack.find({currentLocation.first - 1, currentLocation.second}) == uniquePathStack.end()) {
        pathStack.push(std::make_pair(currentLocation.first - 1, currentLocation.second));
        uniquePathStack.insert(std::make_pair(currentLocation.first - 1, currentLocation.second));
    }
    if (!wallsSensor->isWall(Direction::East) && !houseMap.isVisited(currentLocation.first, currentLocation.second + 1) && uniquePathStack.find({currentLocation.first, currentLocation.second + 1}) == uniquePathStack.end()) {
        pathStack.push(std::make_pair(currentLocation.first, currentLocation.second + 1));
        uniquePathStack.insert(std::make_pair(currentLocation.first, currentLocation.second + 1));
    }
    if (!wallsSensor->isWall(Direction::South) && !houseMap.isVisited(currentLocation.first + 1, currentLocation.second) && uniquePathStack.find({currentLocation.first + 1, currentLocation.second}) == uniquePathStack.end()) {
        pathStack.push(std::make_pair(currentLocation.first + 1, currentLocation.second));
        uniquePathStack.insert(std::make_pair(currentLocation.first + 1, currentLocation.second));
    }
    if (!wallsSensor->isWall(Direction::West) && !houseMap.isVisited(currentLocation.first, currentLocation.second - 1) && uniquePathStack.find({currentLocation.first, currentLocation.second - 1}) == uniquePathStack.end()) {
        pathStack.push(std::make_pair(currentLocation.first, currentLocation.second - 1));
        uniquePathStack.insert(std::make_pair(currentLocation.first, currentLocation.second - 1));
    }
    // }

    if (pathStack.empty()) { //TODO check if this actually works + return to docking station and only then finish
        cout << steps << endl;
        if (houseMap.getVisited().size() == houseMap.getMappedHouse().size()) {
            cout << "Printing path to docking: " << endl;
            printVector(pathToDocking);
            if (currentLocation == DOCK) {
                return Step::Finish;
            }
            return pathToDocking[0];
        } 
    }

    pair<int,int> newTop = pathStack.top();
    vector<Step> pathToNewTop = houseMap.getShortestPath(currentLocation, newTop);
    if (pathToNewTop.size() == 0) {
        cout << "Curr pos: " << currentLocation.first << "," << currentLocation.second << " New top: " << newTop.first << "," << newTop.second << endl;
        cout << "stay3" << endl;
        return Step::Stay; //TODO check  this 
    }
    vector<Step> nextStepToDockingPath = houseMap.getShortestPath(calcNewPosition(static_cast<Direction>(pathToNewTop[0]), currentLocation), DOCK);
    if (nextStepToDockingPath.size() > pathToDocking.size() && (currBattery == pathToDocking.size() + 1 || remainingSteps == pathToDocking.size() + 1)) {
        return pathToDocking[0];
    }
    return pathToNewTop[0];  
}


//TODO what do we do if there is no path
