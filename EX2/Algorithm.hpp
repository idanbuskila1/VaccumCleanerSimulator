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
    unique_ptr<const WallsSensor> walls;
    unique_ptr<const DirtSensor> dirt;
    unique_ptr<const BatteryMeter> battery;

    
public:
    Algorithm(size_t battery):stepsRemaining(0),maxBattery(battery){};
	void setMaxSteps(std::size_t steps) override {stepsRemaining = steps;};
	void setWallsSensor(const WallsSensor& sensor) override {walls=make_unique< const WallsSensorObject>(dynamic_cast<const WallsSensorObject&>(sensor));};
	void setDirtSensor(const DirtSensor& sensor) override {dirt = make_unique< const DirtSensorObject>(dynamic_cast<const DirtSensorObject&>(sensor));};
    void setBatteryMeter(const BatteryMeter& sensor) override{battery = make_unique< const BatteryMeterObject>(dynamic_cast<const BatteryMeterObject&>(sensor));};
	Step nextStep()override{if(maxBattery>1) return Step::East; return Step::West;};//tmp implementation for compiling.
};
#endif