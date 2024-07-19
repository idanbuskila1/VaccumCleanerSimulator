#pragma once

#include "dirt_sensor.h"
#include "enums.hpp"
#include "House.hpp"
#include "RobotState.h"

#include <memory>

class DirtSensorObject : public DirtSensor {
private:
  const House *house_;
  const RobotState *robot_;

public:
  DirtSensorObject(const House &house, const RobotState &robot);
  int dirtLevel() const override;
};
