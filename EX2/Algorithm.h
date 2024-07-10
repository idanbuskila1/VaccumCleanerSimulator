#ifndef ALGORITHM
#define ALGORITHM
#include "abstractClasses.h"

class Algorithm: public AbstractAlgorithm{
    size_t stepsRemaining;
    const size_t maxBattery;
    const WallsSensor* walls;
    const DirtSensor* dirt;
    const BatteryMeter* battery;

    
public:
    Algorithm(int battery):maxBattery(battery),stepsRemaining(0){};
    ~Algorithm () =default;
	void setMaxSteps(std::size_t maxSteps) override {stepsRemaining = maxSteps;};
	void setWallsSensor(const WallsSensor& sensor) override {walls=&sensor;};
	void setDirtSensor(const DirtSensor& sensor)override {dirt = &sensor;};
	void setBatteryMeter(const BatteryMeter& sensor)override {battery=&sensor;};
	Step nextStep()override;
};
#endif