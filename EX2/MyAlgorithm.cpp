#include "MyAlgorithm.hpp"


MyAlgorithm::MyAlgorithm() {
    currentPos = {0,0};
    houseManager.markAsVisited(0, 0, 0);
}

void MyAlgorithm::setBatteryMeter(const BatteryMeter& meter) {
    batteryMeter = &meter;
    maxBattery = batteryMeter->getBatteryState();
    houseManager.setMaxBattery(maxBattery);
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

void printSteps(const vector<Step>& vec) {
    for (const auto& row : vec) {
        std::cout << stepToString(row) << ' ';
    }
    cout << endl;
}
void MyAlgorithm::scanNeighbors(){
    if (!wallsSensor->isWall(Direction::North)&& !houseManager.isVisited(currentPos.first - 1, currentPos.second)) {
        houseManager.setNeedToVisit(currentPos.first - 1, currentPos.second);
    }
    if (!wallsSensor->isWall(Direction::South)&& !houseManager.isVisited(currentPos.first + 1, currentPos.second)) {
        houseManager.setNeedToVisit(currentPos.first + 1, currentPos.second);
    }
    if (!wallsSensor->isWall(Direction::East)&& !houseManager.isVisited(currentPos.first, currentPos.second+1)) {
        houseManager.setNeedToVisit(currentPos.first, currentPos.second + 1);
    }
    if (!wallsSensor->isWall(Direction::West)&& !houseManager.isVisited(currentPos.first, currentPos.second-1)) {
        houseManager.setNeedToVisit(currentPos.first, currentPos.second - 1);
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
        currentPos = calcNewPosition(static_cast<Direction>(ret), currentPos);
        if(!houseManager.isVisited(currentPos.first, currentPos.second)){
            houseManager.addToCellsVisited(currentPos.first, currentPos.second, 0);
        }
        if(houseManager.isNeededVisit(currentPos.first, currentPos.second)){
            houseManager.removeFromNeedVisit(currentPos.first, currentPos.second);
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
    scanNeighbors();
    houseManager.setDirt(currentPos.first, currentPos.second,dirtSensor->dirtLevel());
    if(steps!=0&&houseManager.getTotalDirt()==0 && houseManager.isCellsToVisitEmpty() && currentPos==DOCK){
        return Step::Finish;
    }
    if(state!=AlgoState::INIT){
        return moveByState();
    }
    auto pathToDocking = houseManager.getShortestPath(currentPos, DOCK);
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
        houseManager.setDirt(currentPos.first, currentPos.second,dirtSensor->dirtLevel()-1);
        return Step::Stay;
    }
    // Having charge, maxSteps/battery boundary is not reached and current location is clean- advance to another spot. 
    path = houseManager.getShortestPath(currentPos, DOCK,true);
    if(path.size()==0){
        path = pathToDocking;
        state = AlgoState::TO_DOCK;
        return moveByState();
    }
    state = AlgoState::TO_POS;
    Step ret = moveByState();
    return ret;

}