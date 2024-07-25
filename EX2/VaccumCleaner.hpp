#ifndef VACCUM
#define VACCUM

#include "enums.hpp"
#include <utility>
#include <iostream>

using  std::pair, std::max,  std::min,std::get;
using std::cerr, std::endl, std::cout;

class VaccumCleaner{
    float batterySteps;
    const int maxBatterySteps;
    pair<int,int> curLoc;
    pair<int, int> dockingStation;
    int steps_made = 0;

public:
    VaccumCleaner(const int battery,const pair<int,int> loc):batterySteps(battery),maxBatterySteps(battery),curLoc(loc), dockingStation(loc){};
    int clean();//stays in current loc and cleans dirt for 1 step. return 0 if was succesfull.
    int charge();//stays in current loc and charges for 1 step. return 0 if was succesfull.
    int move(const Direction dir);//moving 1 step towards the direction dir. return 0 if was succesfull.
    float getBatterySteps()const{return batterySteps;}//returns how much batery left currently
    pair<int,int> getCurrentLoc()const{return curLoc;}//returns current location of the vc
    pair<int,int> getDockingLocation()const{return dockingStation;}//returns docking station location
    int getStepsMade()const{return steps_made;}//returns how many steps the vc made
    int getMaxBatterySteps()const{return maxBatterySteps;}//returns max battery steps
}; 
#endif