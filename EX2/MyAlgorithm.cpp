#include "MyAlgorithm.hpp"


MyAlgorithm::MyAlgorithm() {
    currentLocation = {0,0};
    houseMap.markVisited(0, 0, 0);
}

void MyAlgorithm::setBatteryMeter(const BatteryMeter& meter) {
    batteryMeter = &meter;
    maxBattery = batteryMeter->getBatteryState();
    houseMap.setMaxBattery(maxBattery);
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
void MyAlgorithm::updateNeighbors(){
    if (!wallsSensor->isWall(Direction::North)&& !houseMap.isVisited(currentLocation.first - 1, currentLocation.second)) {
        houseMap.markToVisit(currentLocation.first - 1, currentLocation.second);
    }
    if (!wallsSensor->isWall(Direction::East)&& !houseMap.isVisited(currentLocation.first, currentLocation.second+1)) {
        houseMap.markToVisit(currentLocation.first, currentLocation.second + 1);
    }
    if (!wallsSensor->isWall(Direction::South)&& !houseMap.isVisited(currentLocation.first + 1, currentLocation.second)) {
        houseMap.markToVisit(currentLocation.first + 1, currentLocation.second);
    }
    if (!wallsSensor->isWall(Direction::West)&& !houseMap.isVisited(currentLocation.first, currentLocation.second-1)) {
        houseMap.markToVisit(currentLocation.first, currentLocation.second - 1);
    }
}
Step MyAlgorithm::moveByState(){
   Step ret;
    if(wallsSensor->isWall(Direction::North)&&wallsSensor->isWall(Direction::South) &&wallsSensor->isWall(Direction::East)&&wallsSensor->isWall(Direction::West)){//edge case where docking station is srrounded by walls
        return Step::Finish;
    }
    if(state==AlgoState::TO_DOCK || state==AlgoState::TO_POS){
        ret = path.top();
        path.pop();
        if(path.size()==0){
            
            state = state == AlgoState::TO_DOCK? AlgoState::CHARGING:AlgoState::INIT;
        }
        currentLocation = calcNewPosition(static_cast<Direction>(ret), currentLocation);
        if(!houseMap.isVisited(currentLocation.first, currentLocation.second)){
            houseMap.addToVisited(currentLocation.first, currentLocation.second, 0);
        }
        if(houseMap.isInNeedToVisit(currentLocation.first, currentLocation.second)){
            houseMap.eraseFromNeedToVisit(currentLocation.first, currentLocation.second);
        }
        steps++;
        return ret;
    } 
    
    if(state==AlgoState::CHARGING){
        if(batteryMeter->getBatteryState() == maxBattery){
            state = AlgoState::INIT;
            return nextStep();
        }
        else{
            steps++;
            return Step::Stay;
        }
    }
    return nextStep();
}
Step MyAlgorithm::nextStep(){
    updateNeighbors();
    houseMap.setDirt(currentLocation.first, currentLocation.second,dirtSensor->dirtLevel());
    if(steps!=0&&houseMap.getTotalDirt()==0 && houseMap.isNeedToVisitEmpty()){
        return Step::Finish;
    }
    if(state!=AlgoState::INIT){
        return moveByState();
    }
    auto pathToDocking = houseMap.getShortestPath(currentLocation, DOCK);
    if(maxSteps-steps==pathToDocking.size() && pathToDocking.size()>0){
         // The way back to the docking station is exactly as long as the steps left. Go back to docking station and finish (without charging) so we wont fail in mission.
        path = pathToDocking;
        state = AlgoState::TO_DOCK;
        return moveByState();
    }
    if (batteryMeter->getBatteryState() - pathToDocking.size() <2 ) {
        //we have exactly the battery to go back to charge before dying - go 1 step towards docking station.
        if(pathToDocking.size()==0){//edge case where max battery < 1 so the vc cant leave the docking and must charge forever
            return Step::Finish;
        }
        path = pathToDocking;
        state = AlgoState::TO_DOCK;
        return moveByState();
    }
    if (pathToDocking.size()>0 && dirtSensor->dirtLevel()>0&& dirtSensor->dirtLevel()<=9) {//we are on a tile with dirt on it - clean it.
        steps++;
        houseMap.setDirt(currentLocation.first, currentLocation.second,dirtSensor->dirtLevel()-1);
        return Step::Stay;
    }
    // Having charge, maxSteps/battery boundary is not reached and current location is clean- advance to another spot. 
    path = houseMap.getShortestPath(currentLocation, DOCK,true);
    if(path.size()==0){
        return Step::Finish;
    }
    state = AlgoState::TO_POS;
    Step ret = moveByState();
    return ret;

}
/*
Step MyAlgorithm::nextStep() {
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
        steps++;
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
        steps++;
        currentLocation = calcNewPosition(static_cast<Direction>(pathToDocking[0]), currentLocation);
        return pathToDocking[0];
    }

    //we still have dirt in the current cell
    if (currDirt > 0 && currDirt <= 9){
        steps++;
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
            steps++;
            currentLocation = calcNewPosition(static_cast<Direction>(pathToDocking[0]), currentLocation);
            return pathToDocking[0];
        } 
    }

    pair<int,int> newTop = pathStack.top();
    vector<Step> pathToNewTop = houseMap.getShortestPath(currentLocation, newTop);
    if (pathToNewTop.size() == 0) {
        cout << "Curr pos: " << currentLocation.first << "," << currentLocation.second << " New top: " << newTop.first << "," << newTop.second << endl;
        cout << "stay3" << endl;
        steps++;
        return Step::Stay; //TODO check  this 
    }
    vector<Step> nextStepToDockingPath = houseMap.getShortestPath(calcNewPosition(static_cast<Direction>(pathToNewTop[0]), currentLocation), DOCK);
    if (nextStepToDockingPath.size() > pathToDocking.size() && (currBattery == pathToDocking.size() + 1 || remainingSteps == pathToDocking.size() + 1)) {
        steps++;
        currentLocation = calcNewPosition(static_cast<Direction>(pathToDocking[0]), currentLocation);
        return pathToDocking[0];
    }
    steps++;
    currentLocation = calcNewPosition(static_cast<Direction>(pathToNewTop[0]), currentLocation);
    return pathToNewTop[0];  
}

*/
//TODO what do we do if there is no path
