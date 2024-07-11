#ifndef ALGORITHM
#define ALGORITHM
#include "index.hpp"
#include "WallsSensorObject.hpp"
#include "DirtSensorObject.hpp"
#include "BatteryMeterObject.hpp"

class AbstractAlgorithm {
public:
	virtual ~AbstractAlgorithm() {}
	virtual void setMaxSteps(std::size_t maxSteps) = 0;
	virtual void setWallsSensor(const WallsSensor&) = 0;
	virtual void setDirtSensor(const DirtSensor&) = 0;
	virtual void setBatteryMeter(const BatteryMeter&) = 0;
	virtual Step nextStep() = 0;
};

class Algorithm: public AbstractAlgorithm{
    size_t stepsRemaining;
    const size_t maxBattery;
    const WallsSensor* walls;
    const DirtSensor* dirt;
    const BatteryMeter* battery;

    
public:
    Algorithm(int battery):stepsRemaining(0),maxBattery(battery){};
    ~Algorithm () =default;
	void setMaxSteps(std::size_t maxSteps) override {stepsRemaining = maxSteps;};
	void setWallsSensor(const WallsSensor& sensor) override {walls=&sensor;};
	void setDirtSensor(const DirtSensor& sensor)override {dirt = &sensor;};
	void setBatteryMeter(const BatteryMeter& sensor)override {battery=&sensor;};
	Step nextStep()override;
};
#endif