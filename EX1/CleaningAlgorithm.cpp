#include "index.h"

void CleaningAlgorithm::chooseNextDirection(const vector<int> wallSensor){
    int lb=0,ub=3;
    int retDir = (rand() % (ub - lb + 1)) + lb;
    while(wallSensor[retDir])
        retDir = (rand() % (ub - lb + 1)) + lb;
    curDirection = retDir;
}

/*returns:One of the following actions-
    -1:failure- empty battery and not on docking station
    0,1,2,3: move in direction NORTH, EAST, SOUTH, WEST respectively
    4: stay(clean/charge)
*/
int CleaningAlgorithm::getNextMove(int dirtSensor,const int batterySensor,const vector<int>& wallSensor){
    int ret;
    if(wallSensor[0]==1 &&wallSensor[1]==1 &&wallSensor[2]==1 &&wallSensor[3]==1){//edge case where docking station is srrounded by walls
        return STAY;
    }
    if(batterySensor<=0 && pathToDocking.size()>0){//no battery and not on docking station - fail.
        curStep++;
        return -1;
    }
    if(maxSteps-curStep==pathToDocking.size() && pathToDocking.size()>0){
         // The way back to the docking station is exactly as long as the steps left. Go back to docking station and finish (without charging) so we wont fail in mission.
        ret = pathToDocking.top();
        pathToDocking.pop();
        curStep++;
        return ret;
    }
    if(static_cast<int>(pathToDocking.size()) == 0 && batterySensor == static_cast<int>(maxBattery) && isCharging){// if vc is at docking, with full battery and on charging mode - we just finished charging, so disable charging mode.
        isCharging=false;
        curDirection=-1;//force to choose new direction after exiting charge mode.
    }
    if (static_cast<int>(pathToDocking.size()) == 0 && batterySensor < static_cast<int>(maxBattery) && isCharging) {// if vc is at docking, with non-full battery and on charging mode - charge for another step.
        curStep++;
        return STAY;//stay at docking 
    }
    if (batterySensor - pathToDocking.size() <2 ) {
        //we have exactly the battery to go back to charge before dying - go 1 step towards docking station.
        curStep++;
        if(pathToDocking.size()==0){//edge case where max battery < 1 so the vc cant leave the docking and must charge forever
            return STAY;
        }
        ret = pathToDocking.top();
        pathToDocking.pop();
        if(pathToDocking.size()==0)//if it was the last step to the docking station, cchange state to charge.
            isCharging=true;
        return ret;
    }
    if (pathToDocking.size()>0 && dirtSensor != DIRT0) {//we are on a tile with dirt on it - clean it.
        curStep++;
        return STAY;
    }
    // Having charge, maxSteps/battery boundary is not reached and current location is clean- advance to another spot. 
    if (curDirection==-1 || wallSensor[curDirection]==1)//if curDirection is null or there is a wall 1 step in the direction curDirection - decide on new direction to move forward safely. else continue with the same direction.
        chooseNextDirection(wallSensor);
    curStep++;
    //update pathToDocking stack
    if(!pathToDocking.empty() && curDirection == pathToDocking.top())//if we are going to make the first step in the stack - pop it.
        pathToDocking.pop();
    else{//push the oposite direction to the stack
        if(curDirection == NORTH) pathToDocking.push(SOUTH);
        else if(curDirection == SOUTH) pathToDocking.push(NORTH);
        else if(curDirection == EAST) pathToDocking.push(WEST);
        else if(curDirection == WEST) pathToDocking.push(EAST);
    }
    // If there is a wall 1 step towards dir in the new location, discard dir. Else vc will continue to go in direction dir.
    return curDirection; //move 1 step in this direction.
}