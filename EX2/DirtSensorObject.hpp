#ifndef DIRT_SENSOR
#define DIRT_SENSOR

#include <memory>

#include "VaccumCleaner.hpp"
#include "House.hpp"
#include "dirt_sensor.h"

class DirtSensorObject: public DirtSensor{
    shared_ptr<VaccumCleaner> vc;
    shared_ptr<House> h;
public:
    DirtSensorObject(shared_ptr<House> house, shared_ptr<VaccumCleaner> vaccum_cleaner):vc(vaccum_cleaner),h(house){};
	int dirtLevel() const override{return h->getDirtLevel(vc->getCurrentLoc());};

};
#endif