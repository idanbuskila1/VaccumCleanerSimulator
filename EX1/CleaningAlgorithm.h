#ifndef ALGO_H
#define ALGO_H


class CleaningAlgorithm{
    const size_t maxBattery;
    const size_t maxSteps;
    size_t curStep;
    int curDirection;
    bool isCharging;//indicates we are in the process of charging to full battery. if false we wont charge - to help the vc go far and not charge too often.
    std::stack<int> pathToDocking; //stack to remember the way 

    void chooseNextDirection(const std::vector<int> wallSensor);//randomly choosing a direction to go next, with no wall ahead.
public:
    CleaningAlgorithm(int battery,int steps):maxBattery(battery),maxSteps(steps),curStep(0),curDirection(-1),isCharging(false),pathToDocking(){};
    int getNextMove(int dirtSensor,const int batterySensor,const std::vector<int>& wallSensor);

};
#endif