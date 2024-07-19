#include "DirtSensorObject.h"

DirtSensorObject::DirtSensorObject(const House &house, const RobotState &robot)
    : house_(&house), robot_(&robot) {}

int DirtSensorObject::dirtLevel() const {
  return house_->getDirtLevel(robot_->getPosition());
}
