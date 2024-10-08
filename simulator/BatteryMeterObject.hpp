#ifndef BATTERY_M_O
#define BATTERY_M_O

#include <memory>

#include "../common/abstract_algorithm.hpp"
#include "VaccumCleaner.hpp"

using std::shared_ptr;

class BatteryMeterObject: public BatteryMeter{
    shared_ptr<VaccumCleaner> vc;

public:
    BatteryMeterObject(shared_ptr<VaccumCleaner> vaccum_cleaner):vc(vaccum_cleaner){};
	std::size_t getBatteryState() const override{return vc->getBatterySteps();};

};
#endif