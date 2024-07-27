#ifndef MY_ALGO_H
#define MY_ALGO_H

#include <vector>
#include <queue>
#include <fstream>
#include <iostream>
#include <stack>
#include <memory> 
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
    HouseManager houseManager;
    size_t steps = 0;
    std::size_t maxBattery = 0;
    std::size_t maxSteps = 0;
    std::stack<Step> path;
    std::pair<int, int> currentPos;
    AlgoState state = AlgoState::INIT;

    void scanNeighbors();
public:
    MyAlgorithm();
    void setBatteryMeter(const BatteryMeter& meter) override;
    void setWallsSensor(const WallsSensor& sensor) override;
    void setDirtSensor(const DirtSensor& sensor) override;
    void setMaxSteps(std::size_t maxSteps) override;
    Step nextStep() override;
    Step moveByState();
    };

#endif // MY_ALGO_H
