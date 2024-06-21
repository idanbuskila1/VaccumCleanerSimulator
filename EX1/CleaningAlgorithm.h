#ifndef ALGO_H
#define ALGO_H

using std::vector, std::tuple, std::string,  std::stack, std::max,  std::min,std::get;
using std::ostringstream, std::to_string, std::ifstream,std::ofstream, std::istringstream;
using std::cerr, std::endl, std::cout;

class CleaningAlgorithm{
    const size_t maxBattery;
    const size_t maxSteps;
    size_t curStep;
    int curDirection;
    bool isCharging;//indicates we are in the process of charging to full battery. if false we wont charge - to help the vc go far and not charge too often.
    stack<int> pathToDocking; //stack to remember the way 

    void chooseNextDirection(const vector<int> wallSensor);//randomly choosing a direction to go next, with no wall ahead.
public:
    CleaningAlgorithm(int battery,int steps):maxBattery(battery),maxSteps(steps),curStep(0),curDirection(-1),isCharging(false),pathToDocking(){};
    int getNextMove(int dirtSensor,const int batterySensor,const vector<int>& wallSensor);//decides on next step based on current state and sensors.

};
#endif