#ifndef DIRT_SENSOR
#define DIRT_SENSOR
#include "abstractClasses.h"
#include "House.h"
#include "VaccumCleaner.h"
class DirtSensorObject: public DirtSensor{
    shared_ptr<House> h;
    shared_ptr<VaccumCleaner> vc;

public:
    DirtSensorObject(shared_ptr<House> house,shared_ptr<VaccumCleaner> vaccumCleaner):h(house),vc(vaccumCleaner){};
    ~DirtSensorObject()=default;
	int dirtLevel() const override{return h->getDirtLevel(vc->getCurrentLoc());};

};
#endif