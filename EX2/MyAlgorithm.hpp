#pragma once

#include "abstract_algorithm.hpp"
#include "DirtSensorObject.hpp"
#include "HouseManager.hpp"
#include "enums.hpp"

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

  // point to dirt
  // std::map<std::pair<int, int>, int> percieved_house_;
  // std::map<std::pair<int, int>, bool> unexplored_points_;
  std::stack<Direction> stack_;

  // methods
  void updateNeighbors();
  // void updateNeighbor(Direction dir);
  bool needCharge();
  // void cleanCurrent();
  Step work();

  // std::stack<Direction> getShortestPath(std::pair<int, int> src,
  //                                       std::pair<int, int> dst,
  //                                       bool search = false);

  // std::vector<std::pair<int, int>> neighbors(std::pair<int, int> point);

public:
  MyAlgorithm();
  MyAlgorithm(AbstractAlgorithm &algorithm);

  void setMaxSteps(std::size_t maxSteps) override;
  void setWallsSensor(const WallsSensor &walls_sensor) override;
  void setDirtSensor(const DirtSensor &dirt_sensor) override;
  void setBatteryMeter(const BatteryMeter &battery_meter) override;
  // TODO : Complete in cpp file
  Step nextStep() override;
};
