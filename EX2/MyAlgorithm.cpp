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

pair<int,int> MyAlgorithm::calcNewPosition(Direction d, pair<int,int> pos) {
    pair<int,int> ret = pos;
    switch (d)
    {
        case Direction::North:
            ret.first = ret.first-1;
            break;
        case Direction::South:
            ret.first = ret.first+1;
            break;
        case Direction::East:
            ret.second = ret.second+1;
            break;
        case Direction::West:
            ret.second = ret.second-1;
            break;    
        default:
            break;
    }
    return ret;
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
        if(path.size()==0){//no path to follow
            state = state == AlgoState::TO_DOCK? AlgoState::CHARGING:AlgoState::INIT;
            return nextStep();
        }
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
    if(steps!=0&&houseMap.getTotalDirt()==0 && houseMap.isNeedToVisitEmpty() && currentLocation==DOCK){
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
    int leftSteps = batteryMeter->getBatteryState() - pathToDocking.size();
    if ( leftSteps<=1 ) {
        if(leftSteps==1 && dirtSensor->dirtLevel()>0 && dirtSensor->dirtLevel()<=9){//we can allow only cleaning and not moving if we got 1 step
            steps++;
            houseMap.setDirt(currentLocation.first, currentLocation.second,dirtSensor->dirtLevel()-1);
            return Step::Stay;
        }
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
    if(path.size()==0){//no more to clean - go to dock and end!
        path = pathToDocking;
        if(path.size()==0&&houseMap.getTotalDirt()==0 && houseMap.isNeedToVisitEmpty()){//allready on dock
            return Step::Finish;
        }
        state = AlgoState::TO_DOCK;
        return moveByState();
    }
    state = AlgoState::TO_POS;
    Step ret = moveByState();
    return ret;

}