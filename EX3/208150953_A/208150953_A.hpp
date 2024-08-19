#ifndef ALGO1
#define ALGO1

#include <stack>
#include <memory>  // For smart pointers
#include "../common/abstract_algorithm.hpp"
#include "../common/enums.hpp"
#include "../common/wall_sensor.hpp"
#include "../common/dirt_sensor.hpp"
#include "../common/battery_meter.hpp"
#include "../simulator/VaccumCleaner.hpp"
#include "../common/HouseMap.h"
#include "../algorithm/AlgorithmRegistration.h"

const pair<int,int> DOCK = {0,0};//dock location in the mapping built is set as the zero point.

class _208150953_A : public AbstractAlgorithm {
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

    pair<int,int> calcNewPosition(Direction d, pair<int,int> pos);
    void updateNeighbors();
    Step moveByState();
public:
    _208150953_A();
    void setBatteryMeter(const BatteryMeter& meter) override;
    void setWallsSensor(const WallsSensor& sensor) override;
    void setDirtSensor(const DirtSensor& sensor) override;
    void setMaxSteps(std::size_t maxSteps) override;
    Step nextStep() override;
};

#endif // ALGO1
