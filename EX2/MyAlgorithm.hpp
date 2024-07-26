#ifndef HOUSE_CLEANING_ALGORITHM_H_
#define HOUSE_CLEANING_ALGORITHM_H_

#include <vector>
#include <queue>
#include <fstream>
#include <iostream>
#include <stack>
#include <memory>  // For smart pointers
#include "abstract_algorithm.hpp"
#include "enums.hpp"
#include "wall_sensor.hpp"
#include "dirt_sensor.hpp"
#include "battery_meter.hpp"
#include "VaccumCleaner.hpp"
#include "HouseMap.h"
#include "utils.h"

class MyAlgorithm : public AbstractAlgorithm {
private:
    const WallsSensor *wallsSensor = nullptr;
    const DirtSensor *dirtSensor = nullptr;
    const BatteryMeter *batteryMeter = nullptr;
    HouseMap houseMap;
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
