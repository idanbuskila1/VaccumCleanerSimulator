#ifndef VACCUM_H
#define VACCUM_H
#include "index.h"
using namespace std;
class VaccumCleaner{
    int batterySteps;
    int maxBatterySteps;
    stack<int> pathToDocking;
    stack<int> history;//all steps history, for output file.
    tuple<int,int> curLoc;
public:
    VaccumCleaner(const int battery,const tuple<int,int> loc):batterySteps(battery),maxBatterySteps(battery),pathToDocking(stack<int>()),history(stack<int>()),curLoc(loc){};
    int goCharge(int maxStepsAllowed,bool dontCharge=false);//gets back to docking station according to pathToDocking, and stays there until 100% charged. if reached steps limit stops. returns number of steps taken in the process.
    void clean(House& h);//stays in current loc and cleans dirt for 1 step.
    void move(int dir,bool isReturnToCharge=false);//moving 1 step towards the direction dir
    int getBatterySteps(){return batterySteps;}
    tuple<int,int> getCurrentLoc(){return curLoc;}
    int getStepsFromDocking(){return pathToDocking.size();}
};
#endif