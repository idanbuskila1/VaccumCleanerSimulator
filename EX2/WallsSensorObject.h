#pragma once

#include "wall_sensor.h"
#include "enums.hpp"
#include "House.hpp"
#include "RobotState.h"
#include "Utils.h"

#include <iostream>
#include <memory>

class WallsSensorObject : public WallsSensor {
private:
  const House *house_;
  const RobotState *robot_;

public:
  WallsSensorObject();
  WallsSensorObject(const House &house, const RobotState &robot);
  bool isWall(Direction d) const override;
};
