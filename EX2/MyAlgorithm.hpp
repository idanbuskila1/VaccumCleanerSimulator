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
    VaccumCleaner *robotState;
    const WallsSensor *wallsSensor = nullptr;
    const DirtSensor *dirtSensor = nullptr;
    const BatteryMeter *batteryMeter = nullptr;
    HouseMap houseMap;
    std::size_t maxSteps = 0;
    
    std::stack<std::pair<int,int>> pathStack;
    std::set<std::pair<int, int>> uniquePathStack;
public:
    MyAlgorithm();
    MyAlgorithm(VaccumCleaner &state);
    void setBatteryMeter(const BatteryMeter& meter) override;
    void setWallsSensor(const WallsSensor& sensor) override;
    void setDirtSensor(const DirtSensor& sensor) override;
    void setMaxSteps(std::size_t maxSteps) override;
    Direction nextStep() override;
    // int getDirtLeft() const;
};

#endif // HOUSE_CLEANING_ALGORITHM_H_
