#include <memory>

#include "abstract_algorithm.h"
#include "enums.hpp"
#include "RobotState.h"
#include "battery_meter.h"
#include "House.hpp"

class BatteryMeterObject : public BatteryMeter {
private:
  const RobotState *robot_;

public:
  BatteryMeterObject(const RobotState &robotState);
  std::size_t getBatteryState() const override;
};