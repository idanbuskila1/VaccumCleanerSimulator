#pragma once

#include "MyAlgorithm.h"
#include "abstract_algorithm.h"
#include "BatteryMeterObject.h"
#include "DirtSensorObject.h"
#include "RobotState.h"
#include "Utils.h"
#include "WallsSensorObject.h"

#include <fstream>
#include <iostream>

class Simulator {
private:
  std::size_t max_steps_;
  std::size_t steps_ = 0;
  std::string final_state_ = "";
  House house_;
  RobotState robot_state_;
  std::vector<char> step_list_;
  AbstractAlgorithm *algo;
  // sensors
  DirtSensorObject dirt_sensor_;
  WallsSensorObject wall_sensor_;
  BatteryMeterObject battery_meter_;

  int initSensors();

public:
  Simulator();
  int readHouseFile(const std::string &house_file_path);
  void setAlgorithm(AbstractAlgorithm &algorithm);
  void run();
  void dump(std::string outFileName);
};
