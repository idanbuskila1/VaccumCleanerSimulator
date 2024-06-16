#ifndef VACCUM_H
#define VACCUM_H
#include "index.h"
using namespace std;
class VaccumCleaner{
    float batterySteps;
    int maxBatterySteps;
    stack<int> pathToDocking;
    stack<int> history;//all steps history, for output file.
    tuple<int,int> curLoc;
public:
    VaccumCleaner(const int battery,const tuple<int,int> loc):batterySteps(battery),maxBatterySteps(battery),pathToDocking(stack<int>()),history(stack<int>()),curLoc(loc){};
    void stay(House& h);//stays in current loc and cleans dirt for 1 step or charging.
    void move(int dir);//moving 1 step towards the direction dir
    float getChargePerStep(){return (float)(maxBatterySteps)/20;}//return the amount of charging obtained by 1 stay at docking station.
    float getBatterySteps(){return batterySteps;}//returns how much batery left currently
    int getMaxBatterySteps(){return maxBatterySteps;} //returns the max battery allowed
    tuple<int,int> getCurrentLoc(){return curLoc;}//returns current location of the vc
    //int getStepsFromDocking(){return pathToDocking.size();}
    //int goCharge(int maxStepsAllowed,bool dontCharge=false);//gets back to docking station according to pathToDocking, and stays there until 100% charged. if reached steps limit stops. returns number of steps taken in the process.
}; 
#endif