#include "VaccumCleaner.hpp"
int VaccumCleaner::clean(){
    if(batterySteps<1)//no battery for cleaning
            return 1;
    this->batterySteps--; 
    return 0;
}
int VaccumCleaner::charge(){
    float chargePerStep = (float)(maxBatterySteps)/20;
    this->batterySteps=min(this->batterySteps+chargePerStep,(float)maxBatterySteps);//dont overflow max battery.
    return 0;
}
//according to  dir, move 1 step towards the direction.
int VaccumCleaner::move(Direction dir){
    if(batterySteps<1)
        return 1;
    this->batterySteps--;
    this->steps_made++;
    if(dir == Direction::North){
        this->curLoc = {this->curLoc.first-1,this->curLoc.second};
        return 0;
    }
    if(dir == Direction::South){
        this->curLoc = {this->curLoc.first+1,this->curLoc.second};
        return 0;
    }
    if(dir == Direction::East){
        this->curLoc = {this->curLoc.first,this->curLoc.second+1};
        return 0;
    }
    if(dir == Direction::West){
        this->curLoc = {this->curLoc.first,this->curLoc.second-1};
        return 0;
    }
    return 1;
}