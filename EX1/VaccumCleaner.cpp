#include "index.h"
int VaccumCleaner::stay(House& h){
    if(get<0>(this->curLoc)==get<0>(h.getDockingStationLoc()) && get<1>(this->curLoc)==get<1>(h.getDockingStationLoc()) ){//stay in docking station means charging
        float chargePerStep = (float)(maxBatterySteps)/20;
        this->batterySteps=min(this->batterySteps+chargePerStep,(float)maxBatterySteps);//dont overflow max battery.
        //this->history.push(CHARGE);
    }
    else{//stay for cleaning
        if(batterySteps<1)//no battery for cleaning
            return 1;
        h.updateCleaningState(this->curLoc);
        this->batterySteps--; 
        //this->history.push(CLEAN);
    }
    return 0;
} 
//according to  dir, move 1 step towards the direction.
int VaccumCleaner::move(int dir){
    if(batterySteps<1)
        return 1;
    this->batterySteps--;
    //this->history.push(dir);
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

vector<string>* VaccumCleaner::cleanHouse(House& h, CleaningAlgorithm& alg, int maxSteps) {
    int steps = 0, action;
    int dirtSensor;
    tuple<int, int> curLoc;
    vector<string> directionsTranslate{"North", "East", "South","West"};
    vector<int> wallSensor{0, 0, 0, 0}; // wallSensor[i] == 1 if there is a wall in direction dir, where NORTH=0, EAST=1, SOUTH=2, WEST=3
    vector<string>* stepLog = new vector<string>(); // Vector to store steps log

    while (steps < maxSteps) {
        curLoc = getCurrentLoc();
        if (curLoc == h.getDockingStationLoc() && h.getTotalDirtLeft() == 0) {
            /*string logMessage = "finished with success: house is clean and the vacuum cleaner is at docking station";
            cout << logMessage << endl;
            stepLog->push_back(logMessage);*/
            break;
        }
        
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
            string logMessage = "failure. battery is empty and not on docking station";
            //cout << logMessage << endl;
            stepLog->push_back(logMessage);
            break;
        }
        
        if (action == STAY) {
            steps++;
            int err = stay(h);
            if (err) {
                string logMessage = "failure. algorithm tried to make vacuum cleaner clean with no battery.";
                // << logMessage << endl;
                stepLog->push_back(logMessage);
                break;
            }
            if (curLoc == h.getDockingStationLoc()) {
                string logMessage = "step: " + to_string(steps) + ". battery: " + to_string(getBatterySteps()) + ". current location: [" + to_string(get<0>(curLoc)) + "," + to_string(get<1>(curLoc)) + "]. action: charge";
                //cout << logMessage << endl;
                stepLog->push_back(logMessage);
            } else {
                string logMessage = "step: " + to_string(steps) + ". battery: " + to_string(getBatterySteps()) + ". current location: [" + to_string(get<0>(curLoc)) + "," + to_string(get<1>(curLoc)) + "]. action: clean.";
                //cout << logMessage << endl;
                stepLog->push_back(logMessage);
            }
            continue;
        } else { // need to advance 1 step in the direction returned with action
            steps++;
            if (h.isWallInDirection(action, curLoc)) {
                string logMessage = "failure. algorithm tried to move vacuum cleaner into a wall.";
                //cout << logMessage << endl;
                stepLog->push_back(logMessage);
                break;
            }
            int err = move(action);
            if (err) {
                string logMessage = "failure. algorithm tried to move vacuum cleaner with no battery.";
                //cout << logMessage << endl;
                stepLog->push_back(logMessage);
                break;
            }
            string logMessage = "step: " + to_string(steps) + ". battery: " + to_string(getBatterySteps()) + ". current location: [" + to_string(get<0>(curLoc)) + "," + to_string(get<1>(curLoc)) + "]. action: move in direction " + directionsTranslate[action];
            //cout << logMessage << endl;
            stepLog->push_back(logMessage);
            continue;
        }
    }
    
    /*string logMessage = "dirt left: " + to_string(h.getTotalDirtLeft()) + ".";
    cout << logMessage << endl;
    stepLog->push_back(logMessage);*/
    return stepLog; // for printing to out file in main.
}