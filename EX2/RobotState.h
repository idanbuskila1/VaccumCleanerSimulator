#pragma once

#include "enums.hpp"
#include "Utils.h"

#include <algorithm>

using std::pair;

class RobotState {
private:
  double max_battery_;
  double battery_ = 0;
  int steps_to_full_charge_ = MAX_STEPS_CHARGE;
  pair<int,int> robot_pos_;

public:
  RobotState();
  ~RobotState();
  void init(double battery, pair<int,int> loc);
  double maxBattery() const;
  double battery() const;
  void step(Step stepDiretion);
  void charge();

  pair<int,int> getPosition() const;
};
