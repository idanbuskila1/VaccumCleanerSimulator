#ifndef VACCUM_H
#define VACCUM_H

using std::vector, std::tuple, std::string,  std::stack, std::max,  std::min,std::get;
using std::ostringstream, std::to_string, std::ifstream,std::ofstream, std::istringstream;
using std::cerr, std::endl, std::cout;

class VaccumCleaner{
    float batterySteps;
    int maxBatterySteps;
    tuple<int,int> curLoc;

public:
    VaccumCleaner(const int battery,const tuple<int,int> loc):batterySteps(battery),maxBatterySteps(battery),curLoc(loc){};
    vector<string>* cleanHouse(House& h,CleaningAlgorithm& alg, const int maxSteps);//clean the house according to the algorithm.
    int stay(House& h);//stays in current loc and cleans dirt for 1 step or charging. return 0 if was succesfull.
    int move(const int dir);//moving 1 step towards the direction dir. return 0 if was succesfull.
    float getBatterySteps()const{return batterySteps;}//returns how much batery left currently
    tuple<int,int> getCurrentLoc()const{return curLoc;}//returns current location of the vc
}; 
#endif