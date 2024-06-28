#include "index.h"
int VaccumCleaner::stay(House& h){
    if(get<0>(this->curLoc)==get<0>(h.getDockingStationLoc()) && get<1>(this->curLoc)==get<1>(h.getDockingStationLoc()) ){//stay in docking station means charging
        float chargePerStep = (float)(maxBatterySteps)/20;
        this->batterySteps=min(this->batterySteps+chargePerStep,(float)maxBatterySteps);//dont overflow max battery.
    }
    else{//stay for cleaning
        if(batterySteps<1)//no battery for cleaning
            return 1;
        h.updateCleaningState(this->curLoc);
        this->batterySteps--; 
    }
    return 0;
} 
//according to  dir, move 1 step towards the direction.
int VaccumCleaner::move(int dir){
    if(batterySteps<1)
        return 1;
    this->batterySteps--;
    if(dir == NORTH){
        get<0>(this->curLoc)=get<0>(this->curLoc)-1;
        return 0;
    }
    if(dir == SOUTH){
        get<0>(this->curLoc)=get<0>(this->curLoc)+1;
        return 0;
    }
    if(dir == EAST){
        get<1>(this->curLoc)=get<1>(this->curLoc)+1;
        return 0;
    }
    if(dir == WEST){
        get<1>(this->curLoc)=get<1>(this->curLoc)-1;
        return 0;
    }
    return 1;
}

const vector<string>* VaccumCleaner::cleanHouse(House& h, CleaningAlgorithm& alg, int maxSteps) {
    int steps = 0, action;
    int dirtSensor;
    vector<string> directionsTranslate{"North", "East", "South","West"};
    vector<int> wallSensor{0, 0, 0, 0}; // wallSensor[i] == 1 if there is a wall in direction dir, where NORTH=0, EAST=1, SOUTH=2, WEST=3
    vector<string>* stepLog = new vector<string>(); // Vector to store steps log
    stepLog->reserve(maxSteps);
    string logMessage;

    while (steps < maxSteps) {
        if (curLoc == h.getDockingStationLoc() && h.getTotalDirtLeft() == 0)//success
            break;
        
        // Initialize arguments for next algorithm move decision
        dirtSensor = h.getDirtLevel(curLoc);
        wallSensor[NORTH] = h.isWallInDirection(NORTH, curLoc);
        wallSensor[SOUTH] = h.isWallInDirection(SOUTH, curLoc);
        wallSensor[WEST] = h.isWallInDirection(WEST, curLoc);
        wallSensor[EAST] = h.isWallInDirection(EAST, curLoc);
        
        // Ask algorithm for next move decision
        action = alg.getNextMove(dirtSensor, getBatterySteps(), wallSensor);
        
        // Perform the move we got from the algorithm
        if (action == -1) {
            logMessage = "failure. battery is empty and not on docking station";
            stepLog->push_back(logMessage);
            break;
        }
        
        if (action == STAY) {
            steps++;
            if (curLoc == h.getDockingStationLoc()) {//staying on docking is charging
                logMessage = "step: " + to_string(steps) + ". battery: " + to_string(getBatterySteps()) + ". current location: [" + to_string(get<0>(curLoc)) + "," + to_string(get<1>(curLoc)) + "]. action: charge";
                stepLog->push_back(logMessage);
            } else {//staying elsewhere is cleaning
                logMessage = "step: " + to_string(steps) + ". battery: " + to_string(getBatterySteps()) + ". current location: [" + to_string(get<0>(curLoc)) + "," + to_string(get<1>(curLoc)) + "]. action: clean.";
                stepLog->push_back(logMessage);
            }
            int err = stay(h);
            if (err) {
                logMessage = "failure. algorithm tried to make vacuum cleaner clean with no battery.";
                stepLog->push_back(logMessage);
                break;
            }
            continue;
        } else { // need to advance 1 step in the direction returned with action
            steps++;
            logMessage = "step: " + to_string(steps) + ". battery: " + to_string(getBatterySteps()) + ". current location: [" + to_string(get<0>(curLoc)) + "," + to_string(get<1>(curLoc)) + "]. action: move in direction " + directionsTranslate[action];
            stepLog->push_back(logMessage);
            if (h.isWallInDirection(action, curLoc)) {
                logMessage = "failure. algorithm tried to move vacuum cleaner into a wall.";
                stepLog->push_back(logMessage);
                break;
            }
            int err = move(action);
            if (err) {
                logMessage = "failure. algorithm tried to move vacuum cleaner with no battery.";
                stepLog->push_back(logMessage);
                break;
            } 
            continue;
        }
    }
    return stepLog; // for printing to out file in main.
}