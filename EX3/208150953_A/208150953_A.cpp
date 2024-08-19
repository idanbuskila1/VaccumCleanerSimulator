#include "208150953_A.hpp"

REGISTER_ALGORITHM (_208150953_A);

_208150953_A::_208150953_A() {
    currentLocation = {0,0};
    houseMap.markVisited(0, 0, 0);
}

void _208150953_A::setBatteryMeter(const BatteryMeter& meter) {
    batteryMeter = &meter;
    maxBattery = batteryMeter->getBatteryState();
    houseMap.setMaxBattery(maxBattery);
}

void _208150953_A::setWallsSensor(const WallsSensor& sensor) {
    wallsSensor = &sensor;
}

void _208150953_A::setDirtSensor(const DirtSensor& sensor) {
    dirtSensor = &sensor;
}

void _208150953_A::setMaxSteps(std::size_t max) {
    maxSteps = max;
}

pair<int,int> _208150953_A::calcNewPosition(Direction d, pair<int,int> pos) {
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

void _208150953_A::updateNeighbors(){
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
Step _208150953_A::moveByState(){
   Step ret;
    if(wallsSensor->isWall(Direction::North)&&wallsSensor->isWall(Direction::South) &&wallsSensor->isWall(Direction::East)&&wallsSensor->isWall(Direction::West)){//edge case where docking station is srrounded by walls
        return Step::Finish;
    }
    //if we are on way to a position but battery is low- go back to docking station
    auto pathToDocking = houseMap.getShortestPath(currentLocation, DOCK);
    int leftSteps = batteryMeter->getBatteryState() - pathToDocking.size();
    if(state==AlgoState::TO_POS && leftSteps<=1){
        path=pathToDocking;
        state = AlgoState::TO_DOCK;
    }
    //if we are on way to a position but ran out of steps- go back to docking station 
    if(state==AlgoState::TO_POS && maxSteps-steps==pathToDocking.size()){
        if(pathToDocking.size()==0){
            return Step::Finish;
        }
        path=pathToDocking;
        state = AlgoState::TO_POS;
    }
    //if we are on a way to somewhere
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
    //if in charge mode
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
Step _208150953_A::nextStep(){
    //if we are done with the steps, return finish if made it to dock in time, else return stay as default val to the simulator.
    if(steps==maxSteps){
        if(currentLocation==DOCK){
            return Step::Finish;
        }
        return Step::Stay;
    }
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