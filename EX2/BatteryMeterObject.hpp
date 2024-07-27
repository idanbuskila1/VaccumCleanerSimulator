#ifndef BATTERY_METER
#define BATTERY_METER

#include <memory>

#include "abstract_algorithm.h"
#include "VaccumCleaner.hpp"

using std::shared_ptr;

class BatteryMeterObject: public BatteryMeter{
    shared_ptr<VaccumCleaner> vc;

public:
    BatteryMeterObject(shared_ptr<VaccumCleaner> vaccum_cleaner):vc(vaccum_cleaner){};
	std::size_t getBatteryState() const override{return vc->getBatterySteps();};

};
#endif