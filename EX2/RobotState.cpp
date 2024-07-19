#include "RobotState.h"

RobotState::RobotState() {}

RobotState::~RobotState() {}

void RobotState::init(double battery, pair<int,int> loc) {
  battery_ = max_battery_ = battery;
  steps_to_full_charge_ = 20;
  robot_pos_ = loc;
}

double RobotState::maxBattery() const { return max_battery_; }

double RobotState::battery() const { return battery_; }

void RobotState::step(Step stepDirection) {
  if (battery_)
    battery_--;
  else
    return;
    switch (stepDirection) {
    case Step::North:
      this->robot_pos_.first -= 1;
    case Step::South:
      this->robot_pos_.first += 1;
    case Step::West:
      this->robot_pos_.second -= 1;
    case Step::East:
      this->robot_pos_.second += 1;
    default:
      this->robot_pos_.second += 1;

    }
};

void RobotState::charge() {
  battery_ += max_battery_ / steps_to_full_charge_;
  battery_ = std::min(battery_, max_battery_);
}

pair<int,int> RobotState::getPosition() const { return robot_pos_; }
