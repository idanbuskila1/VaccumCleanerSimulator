#include "index.h"
void VaccumCleaner::clean(House& h){
    h.updateCleaningState(this->curLoc);
    this->batterySteps--;
    this->history.push(CLEAN);
}
//according to  dir, move 1 step towards the direction, and update path to docking.
void VaccumCleaner::move(int dir, bool isReturnToCharge){
    this->batterySteps--;
    if(dir == NORTH){
        get<0>(this->curLoc)=get<0>(this->curLoc)+1;
        this->history.push(NORTH);
        if(!isReturnToCharge){//not on its way to the docking station. a regular move.
            if(!this->pathToDocking.empty() && dir == this->pathToDocking.top())
                this->pathToDocking.pop();
            else
                this->pathToDocking.push(SOUTH);
        }
        return;
    }
    if(dir == SOUTH){
        get<0>(this->curLoc)=get<0>(this->curLoc)-1;
        if(!isReturnToCharge){
            if(!this->pathToDocking.empty() && dir == this->pathToDocking.top())
                this->pathToDocking.pop();
            else
                this->pathToDocking.push(NORTH);
        }
        this->history.push(SOUTH);
        return;
    }
    if(dir == EAST){
        get<1>(this->curLoc)=get<1>(this->curLoc)+1;
        this->history.push(EAST);
        if(!isReturnToCharge){
            if(!this->pathToDocking.empty() && dir == this->pathToDocking.top())
                this->pathToDocking.pop();
            else
                this->pathToDocking.push(WEST);
        }
        return;
    }
    if(dir == WEST){
        get<1>(this->curLoc)=get<1>(this->curLoc)-1;
        this->history.push(WEST);
        if(!isReturnToCharge){
            if(!this->pathToDocking.empty() && dir == this->pathToDocking.top())
                this->pathToDocking.pop();
            else
                this->pathToDocking.push(EAST);
        }
        return;
    }
}
int VaccumCleaner::goCharge(int maxStepsAllowed,bool dontCharge){
    stack<int>& path = this->pathToDocking;
    int curDir;
    int stepsToDocking = path.size();
    //reach docking station.
    while (path.size()>0 && maxStepsAllowed>0)
    {
        curDir=path.top();
        this->move(curDir,true);
        path.pop();
        maxStepsAllowed--;
    }
    if(maxStepsAllowed==0) return stepsToDocking-path.size();//if we stoped because steps limit, stop and return the steps we made.
    if(dontCharge) return stepsToDocking;//dont charge, only go back to docking.
    //stay until full battery or steps limit is reached.
    /*int stepsToFull =floor((this->maxBatterySteps - this->batterySteps)*20/this->maxBatterySteps);
    int stepsStayed= 0;
    while(stepsToFull>0 && maxStepsAllowed>0){
        this->history.push(CHARGE);
        stepsStayed++;
        stepsToFull--;
        maxStepsAllowed--;
    }
    this->batterySteps +=floor(stepsStayed*(this->maxBatterySteps/20));*/
    int stepsStayed= 0;
    int chargePerStep = floor(this->maxBatterySteps/20);
    while(maxStepsAllowed>0 && this->batterySteps+chargePerStep<this->maxBatterySteps){
        this->history.push(CHARGE);
        stepsStayed++;
        maxStepsAllowed--;
        this->batterySteps+=chargePerStep;
    }
    return stepsToDocking + stepsStayed;
}
    
