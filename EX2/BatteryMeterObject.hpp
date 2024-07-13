#ifndef BATTERY_METER
#define BATTERY_METER

#include <memory>

#include "abstract_algorithm.hpp"
#include "Simulator.hpp"

class BatteryMeterObject: public BatteryMeter{
    std::shared_ptr<Simulator> sim;

public:
    BatteryMeterObject(std::shared_ptr<Simulator> simulator):sim(simulator){};
	std::size_t getBatteryState() const override{return this->sim->getBattery();};//implicit casting to size_t

};
#endif