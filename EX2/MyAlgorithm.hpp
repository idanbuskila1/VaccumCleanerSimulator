#ifndef HOUSE_CLEANING_ALGORITHM_H_
#define HOUSE_CLEANING_ALGORITHM_H_

#include <vector>
#include <queue>
#include <fstream>
#include <iostream>
#include <stack>
#include <memory>  // For smart pointers
#include "abstract_algorithm.h"
#include "enums.h"
#include "wall_sensor.h"
#include "dirt_sensor.h"
#include "battery_meter.h"
#include "VaccumCleaner.hpp"
#include "HouseManager.h"
#include "utils.h"

class MyAlgorithm : public AbstractAlgorithm {
private:
    const WallsSensor *wallsSensor = nullptr;
    const DirtSensor *dirtSensor = nullptr;
    const BatteryMeter *batteryMeter = nullptr;
    HouseManager houseMap;
    size_t steps = 0;
    std::size_t maxSteps = 0;
    std::size_t maxBattery = 0;
    std::pair<int, int> currentLocation;
    std::stack<Step> path;
    AlgoState state = AlgoState::INIT;

    void updateNeighbors();
public:
    MyAlgorithm();
    void setBatteryMeter(const BatteryMeter& meter) override;
    void setWallsSensor(const WallsSensor& sensor) override;
    void setDirtSensor(const DirtSensor& sensor) override;
    void setMaxSteps(std::size_t maxSteps) override;
    Step nextStep() override;
    Step moveByState();
    // int getDirtLeft() const;
};

#endif // HOUSE_CLEANING_ALGORITHM_H_
