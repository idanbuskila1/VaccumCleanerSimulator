#include "MyAlgorithm.hpp"

MyAlgorithm::MyAlgorithm() : robotState(nullptr){}

MyAlgorithm::MyAlgorithm(VaccumCleaner &state) {
    this->robotState = &state;
    pathStack.push(robotState->getDockingLocation());
    uniquePathStack.insert(robotState->getDockingLocation());
    houseMap.setDockingLocation(robotState->getDockingLocation());
}

void MyAlgorithm::setBatteryMeter(const BatteryMeter& meter) {
    batteryMeter = &meter;
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

void printVector(const vector<Direction>& vec) {
    for (const auto& row : vec) {
        std::cout << stepToString(row) << ' ';
    }
    cout << endl;
}

Direction MyAlgorithm::nextStep() {
    
    if (robotState->getStepsMade() >= 360) {
        cout << "Battery: " << robotState->getBatterySteps() << ", Counter steps: "<< robotState->getStepsMade() << endl;
    }

    if (pathStack.empty()) { 
        std::cout << "pathstack empty" << endl;
        if (houseMap.getVisited().size() == houseMap.getMappedHouse().size()) {
            std::cout << "visited size: " << houseMap.getVisited().size() << " mapped size: " << houseMap.getMappedHouse().size() << endl;
            if (robotState->getCurrentLoc() == robotState->getDockingLocation()) {
                return Direction::Finish;
            }
        }
    }

    if (robotState->getStepsMade() >= int(maxSteps)) {
        std::cout << "steps made: " << robotState->getStepsMade() << " max steps: " << maxSteps << endl;
        return Direction::Finish;
    }
    //i check my neighbors and see who is a wall and add myself and my neighbors to the seen map
    if (batteryMeter->getBatteryState() != (size_t)robotState->getMaxBatterySteps() && robotState->getCurrentLoc() == robotState->getDockingLocation()) {
        return Direction::Stay;
    } 

    std::pair<int,int> currPos = robotState->getCurrentLoc();
    houseMap.markSeen(currPos.first, currPos.second);
    if (!wallsSensor->isWall(Direction::North)) {
        houseMap.markSeen(currPos.first - 1, currPos.second);
    }
    if (!wallsSensor->isWall(Direction::East)) {
        houseMap.markSeen(currPos.first, currPos.second + 1);
    }
    if (!wallsSensor->isWall(Direction::South)) {
        houseMap.markSeen(currPos.first + 1, currPos.second);
    }
    if (!wallsSensor->isWall(Direction::West)) {
        houseMap.markSeen(currPos.first, currPos.second - 1);
    }
    int currDirt = dirtSensor->dirtLevel();
    if (currDirt > 0 && currDirt <= 9) {
        if (uniquePathStack.find(currPos) == uniquePathStack.end() && !houseMap.isVisited(currPos.first, currPos.second)) {
            pathStack.push(currPos);
            uniquePathStack.insert(currPos);
        }
    }

    //if i need to start returning to the docking station
    std::size_t currBattery = batteryMeter->getBatteryState();
    vector<Direction> pathToDocking = houseMap.getShortestPath(currPos, robotState->getDockingLocation());
    size_t remainingSteps = maxSteps - robotState->getStepsMade();
    if (currBattery == pathToDocking.size() || remainingSteps == pathToDocking.size())  {
        return pathToDocking[0];
    }

    //we still have dirt in the current cell
    if (currDirt > 0 && currDirt <= 9){
        return Direction::Stay;
    }

    //remove myself from the stack and add myself to the visited map
    // if (!houseMap.isVisited(currPos.first, currPos.second)) {
    houseMap.markVisited(currPos.first, currPos.second, currDirt);
    if (pathStack.top() == robotState->getCurrentLoc()) {
        pathStack.pop();
        uniquePathStack.erase(currPos);
    }
    //add my neighbors to the stack
    if (!wallsSensor->isWall(Direction::North) && !houseMap.isVisited(currPos.first - 1, currPos.second) && uniquePathStack.find({currPos.first - 1, currPos.second}) == uniquePathStack.end()) {
        pathStack.push(std::make_pair(currPos.first - 1, currPos.second));
        uniquePathStack.insert(std::make_pair(currPos.first - 1, currPos.second));
    }
    if (!wallsSensor->isWall(Direction::East) && !houseMap.isVisited(currPos.first, currPos.second + 1) && uniquePathStack.find({currPos.first, currPos.second + 1}) == uniquePathStack.end()) {
        pathStack.push(std::make_pair(currPos.first, currPos.second + 1));
        uniquePathStack.insert(std::make_pair(currPos.first, currPos.second + 1));
    }
    if (!wallsSensor->isWall(Direction::South) && !houseMap.isVisited(currPos.first + 1, currPos.second) && uniquePathStack.find({currPos.first + 1, currPos.second}) == uniquePathStack.end()) {
        pathStack.push(std::make_pair(currPos.first + 1, currPos.second));
        uniquePathStack.insert(std::make_pair(currPos.first + 1, currPos.second));
    }
    if (!wallsSensor->isWall(Direction::West) && !houseMap.isVisited(currPos.first, currPos.second - 1) && uniquePathStack.find({currPos.first, currPos.second - 1}) == uniquePathStack.end()) {
        pathStack.push(std::make_pair(currPos.first, currPos.second - 1));
        uniquePathStack.insert(std::make_pair(currPos.first, currPos.second - 1));
    }
    // }

    if (pathStack.empty()) { //TODO check if this actually works + return to docking station and only then finish
        cout << robotState->getStepsMade() << endl;
        if (houseMap.getVisited().size() == houseMap.getMappedHouse().size()) {
            cout << "Printing path to docking: " << endl;
            printVector(pathToDocking);
            if (currPos == robotState->getDockingLocation()) {
                return Direction::Finish;
            }
            return pathToDocking[0];
        } 
    }

    pair<int,int> newTop = pathStack.top();
    vector<Direction> pathToNewTop = houseMap.getShortestPath(currPos, newTop);
    if (pathToNewTop.size() == 0) {
        cout << "Curr pos: " << currPos.first << "," << currPos.second << " New top: " << newTop.first << "," << newTop.second << endl;
        cout << "stay3" << endl;
        return Direction::Stay; //TODO check  this 
    }
    vector<Direction> nextStepToDockingPath = houseMap.getShortestPath(calcNewPosition(pathToNewTop[0], currPos), robotState->getDockingLocation());
    if (nextStepToDockingPath.size() > pathToDocking.size() && (currBattery == pathToDocking.size() + 1 || remainingSteps == pathToDocking.size() + 1)) {
        return pathToDocking[0];
    }
    return pathToNewTop[0];  
}


//TODO what do we do if there is no path
