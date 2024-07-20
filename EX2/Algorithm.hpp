#ifndef ALGORITHM
#define ALGORITHM

#include "abstract_algorithm.hpp"
#include "WallsSensorObject.hpp"
#include "DirtSensorObject.hpp"
#include "BatteryMeterObject.hpp"

using std::unique_ptr, std::make_unique;

class Algorithm: public AbstractAlgorithm{
    size_t stepsRemaining;
    size_t maxBattery;
    const WallsSensor* walls;
    const DirtSensor* dirt;
    const BatteryMeter* battery;

    
public:
    Algorithm():stepsRemaining(0),maxBattery(0),walls(nullptr),dirt(nullptr),battery(nullptr){};
	void setMaxSteps(std::size_t steps) override {stepsRemaining = steps;};
	void setWallsSensor(const WallsSensor& sensor) override {walls=&sensor;};
	void setDirtSensor(const DirtSensor& sensor) override {dirt =&sensor;};
    void setBatteryMeter(const BatteryMeter& sensor) override{battery =&sensor; maxBattery = battery->getBatteryState();};
	Step nextStep()override{if(dirt->dirtLevel()>0) return Step::Stay; if(battery->getBatteryState()>1 && !walls->isWall(Direction::East)) return  Step::East; else return Step::West;};//tmp implementation for compiling.
};
#endif