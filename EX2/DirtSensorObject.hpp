#ifndef DIRT_SENSOR
#define DIRT_SENSOR
#include "index.hpp"
#include "House.hpp"
#include "VaccumCleaner.hpp"

class DirtSensor {
public:
	virtual ~DirtSensor() {}
	virtual int dirtLevel() const = 0;
};

class DirtSensorObject: public DirtSensor{
    shared_ptr<House> h;
    shared_ptr<VaccumCleaner> vc;

public:
    DirtSensorObject(shared_ptr<House> house,shared_ptr<VaccumCleaner> vaccumCleaner):h(house),vc(vaccumCleaner){};
    ~DirtSensorObject()=default;
	int dirtLevel() const override{return h->getDirtLevel(vc->getCurrentLoc());};

};
#endif