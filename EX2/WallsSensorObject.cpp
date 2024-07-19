#include "WallsSensorObject.h"
#include "AlgoUtils.h"

WallsSensorObject::WallsSensorObject(const House &house, const RobotState &robot)
    : house_(&house), robot_(&robot) {}

bool WallsSensorObject::isWall(Direction direction) const {

  return house_->isWall(
    // print the current position and the direction
      getNextPosition(robot_->getPosition(), static_cast<Step>(direction)));
}
