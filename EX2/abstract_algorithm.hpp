#ifndef ROBOT_ALGORITHM_H__
#define ROBOT_ALGORITHM_H__

#include <cstddef>
#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <sstream>

#include "enums.hpp"
#include "battery_meter.hpp"
#include "wall_sensor.hpp"
#include "dirt_sensor.hpp"
#include "VaccumCleaner.hpp"

class AbstractAlgorithm {
public:
	virtual ~AbstractAlgorithm() {}
	virtual void setMaxSteps(std::size_t maxSteps) = 0;
	virtual void setWallsSensor(const WallsSensor&) = 0;
	virtual void setDirtSensor(const DirtSensor&) = 0;
	virtual void setBatteryMeter(const BatteryMeter&) = 0;
	virtual Direction nextStep() = 0;
};

#endif  // ROBOT_ALGORITHM_H__
