#pragma once

#include "abstract_algorithm.h"
#include "DirtSensorObject.h"
#include "HouseManager.h"
#include "types.h"

#include <map>
#include <memory>
#include <stack>

class MyAlgorithm : public AbstractAlgorithm {
private:
  int steps_;
  std::size_t max_steps_;
  AlgoState state_;
  std::size_t max_battery_;
  std::pair<int, int> current_position_;

  const WallsSensor *walls_sensor_ = nullptr;
  const DirtSensor *dirt_sensor_ = nullptr;
  const BatteryMeter *battery_meter_ = nullptr;

  HouseManager house_manager_;

  std::stack<Direction> stack_;

  // methods
  void updateNeighbors();
  bool needCharge();
  Step work();
  
public:
  MyAlgorithm();
  MyAlgorithm(AbstractAlgorithm &algorithm);

  void setMaxSteps(std::size_t maxSteps) override;
  void setWallsSensor(const WallsSensor &walls_sensor) override;
  void setDirtSensor(const DirtSensor &dirt_sensor) override;
  void setBatteryMeter(const BatteryMeter &battery_meter) override;
  Step nextStep() override;
};
