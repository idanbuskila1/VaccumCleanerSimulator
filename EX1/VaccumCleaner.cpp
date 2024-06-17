#include "index.h"
void VaccumCleaner::stay(House& h){
    if(std::get<0>(this->curLoc)==std::get<0>(h.getDockingStationLoc()) && std::get<1>(this->curLoc)==std::get<1>(h.getDockingStationLoc()) ){//stay in docking station means charging
        float chargePerStep = this->getChargePerStep();
        this->batterySteps=std::min(this->batterySteps+chargePerStep,(float)this->maxBatterySteps);//dont overflow max battery.
        //this->history.push(CHARGE);
    }
    else{//stay for cleaning
        h.updateCleaningState(this->curLoc);
        this->batterySteps--; 
        //this->history.push(CLEAN);
    }
} 
//according to  dir, move 1 step towards the direction.
void VaccumCleaner::move(int dir){
    this->batterySteps--;
    //this->history.push(dir);
    if(dir == NORTH){
        std::get<0>(this->curLoc)=std::get<0>(this->curLoc)+1;
        return;
    }
    if(dir == SOUTH){
        std::get<0>(this->curLoc)=std::get<0>(this->curLoc)-1;
        return;
    }
    if(dir == EAST){
        std::get<1>(this->curLoc)=std::get<1>(this->curLoc)+1;
        return;
    }
    if(dir == WEST){
        std::get<1>(this->curLoc)=std::get<1>(this->curLoc)-1;
        return;
    }
}
/*int VaccumCleaner::goCharge(int maxStepsAllowed,bool dontCharge){
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
    int stepsStayed= 0;
    int chargePerStep = floor(this->maxBatterySteps/20);
    while(maxStepsAllowed>0 && this->batterySteps+chargePerStep<this->maxBatterySteps){
        this->history.push(CHARGE);
        stepsStayed++;
        maxStepsAllowed--;
        this->batterySteps+=chargePerStep;
    }
    return stepsToDocking + stepsStayed;
}*/
    
