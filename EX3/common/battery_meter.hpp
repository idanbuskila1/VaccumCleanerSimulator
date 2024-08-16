#ifndef BATTERY_M
#define BATTERY_M

#include <cstddef>


class BatteryMeter {
public:
	virtual ~BatteryMeter() {}
	virtual std::size_t getBatteryState() const = 0;
};

#endif  // BATTERY_M
