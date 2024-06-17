#ifndef VACCUM_H
#define VACCUM_H

class VaccumCleaner{
    float batterySteps;
    int maxBatterySteps;
    std::tuple<int,int> curLoc;

public:
    VaccumCleaner(const int battery,const std::tuple<int,int> loc):batterySteps(battery),maxBatterySteps(battery),curLoc(loc){};
    std::vector<std::string>* cleanHouse(House& h,CleaningAlgorithm& alg, const int maxSteps);
    int stay(House& h);//stays in current loc and cleans dirt for 1 step or charging. return 0 if was succesfull.
    int move(const int dir);//moving 1 step towards the direction dir. return 0 if was succesfull.
    float getBatterySteps()const{return batterySteps;}//returns how much batery left currently
    std::tuple<int,int> getCurrentLoc()const{return curLoc;}//returns current location of the vc
}; 
#endif