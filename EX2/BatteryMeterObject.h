#ifndef BATTERY_METER
#define BATTERY_METER
#include "abstractClasses.h"
#include "VaccumCleaner.h"
class BatteryMeterObject: public BatteryMeter{
    shared_ptr<VaccumCleaner> vc;

public:
    BatteryMeterObject(shared_ptr<VaccumCleaner> vaccumCleaner):vc(vaccumCleaner){};
    ~BatteryMeterObject() =default;
	std::size_t getBatteryState() const override{return vc->getBatterySteps();};//implicit casting to size_t

};
#endif