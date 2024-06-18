#include "index.h"
int VaccumCleaner::stay(House& h){
    if(std::get<0>(this->curLoc)==std::get<0>(h.getDockingStationLoc()) && std::get<1>(this->curLoc)==std::get<1>(h.getDockingStationLoc()) ){//stay in docking station means charging
        float chargePerStep = (float)(maxBatterySteps)/20;
        this->batterySteps=std::min(this->batterySteps+chargePerStep,(float)maxBatterySteps);//dont overflow max battery.
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
        std::get<0>(this->curLoc)=std::get<0>(this->curLoc)+1;
        return 0;
    }
    if(dir == SOUTH){
        std::get<0>(this->curLoc)=std::get<0>(this->curLoc)-1;
        return 0;
    }
    if(dir == EAST){
        std::get<1>(this->curLoc)=std::get<1>(this->curLoc)+1;
        return 0;
    }
    if(dir == WEST){
        std::get<1>(this->curLoc)=std::get<1>(this->curLoc)-1;
        return 0;
    }
    return 1;
}
std::vector<std::string>* VaccumCleaner::cleanHouse(House& h,CleaningAlgorithm& alg,int maxSteps){
    int steps=0,action;
    int dirtSensor;
    std::tuple<int,int> curLoc;
    std::vector<int> wallSensor{0,0,0,0};//wallSensor[i]==1 iff there is a wall in direction dir, where NORTH=0, EAST=1, SOUTH=2, WEST=3
    std::vector<std::string>* stepLog = new std::vector<std::string>(); // Vector to store steps log
    while(steps<maxSteps){
        curLoc = getCurrentLoc();
        if(getCurrentLoc()==h.getDockingStationLoc() && h.getTotalDirtLeft()==0){
            std::cout<<"finished with success: house is clean and the vaccum cleaner is at docking station"<<std::endl;
            break;
        }
        //initialize arguments for next algorithm move decision
        dirtSensor = h.getDirtLevel(curLoc);
        wallSensor[NORTH] = h.isWallInDirection(NORTH,curLoc);
        wallSensor[SOUTH] = h.isWallInDirection(SOUTH,curLoc);
        wallSensor[WEST] = h.isWallInDirection(WEST,curLoc);
        wallSensor[EAST] = h.isWallInDirection(EAST,curLoc);
        //ask algorithm for next move decision
        action = alg.getNextMove(dirtSensor,getBatterySteps(),wallSensor);
        //perform the move we got from the algorithm
        if(action == -1){
            std::cout << "failure. battery is empty and not on docking station" << std::endl;
            break;
        }
        if(action == STAY){
            steps++;
            int err = stay(h);
            if(err){
                std::cout<<"failure. algorithm tried to make vaccum cleaner clean with no battery."<<std::endl;
                break;
            }
            if(curLoc == h.getDockingStationLoc()){
                std::cout<<"step: "<<steps<<"."<<"battery: "<<getBatterySteps()<<". current location: ["<<std::get<0>(curLoc)<<","<<std::get<1>(curLoc)<<"]. action: charge"<<std::endl;
            }
            else std::cout<<"step: "<<steps<<"."<<"battery: "<<getBatterySteps()<<". current location: ["<<std::get<0>(curLoc)<<","<<std::get<1>(curLoc)<<"]. action: clean."<<std::endl;
            continue;
        }
        else{//need to advance 1 step in the direction returned with action
            steps++;
            if(h.isWallInDirection(action,curLoc)){
                std::cout<<"failure. algorithm tried to move vaccum cleaner into a wall."<<std::endl;
                break;
            }
            int err =move(action);
            if(err){
                std::cout<<"failure. algorithm tried to move vaccum cleaner with no battery."<<std::endl;
                break;
            }
            std::cout<<"step: "<<steps<<"."<<"battery: "<<getBatterySteps()<<". current location: ["<<std::get<0>(curLoc)<<","<<std::get<1>(curLoc)<<"]. action: move in direction "<<action<<std::endl;
            continue;
        }
    }
    std::cout<<"dirt left: "<<h.getTotalDirtLeft()<<"."<<std::endl;
    return stepLog;//for printing to out file in main.
}
