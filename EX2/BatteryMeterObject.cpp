#include "BatteryMeterObject.h"

BatteryMeterObject::BatteryMeterObject(const RobotState &robotState)
    : robot_(&robotState) {}

std::size_t BatteryMeterObject::getBatteryState() const {
  return robot_->battery();
}
