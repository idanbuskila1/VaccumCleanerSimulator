#ifndef DIRT_S_O
#define DIRT_S_O

#include <memory>

#include "VaccumCleaner.hpp"
#include "House.hpp"
#include "../common/dirt_sensor.hpp"

using std::shared_ptr;

class DirtSensorObject: public DirtSensor{
    shared_ptr<VaccumCleaner> vc;
    shared_ptr<House> h;
public:
    DirtSensorObject(shared_ptr<House> house, shared_ptr<VaccumCleaner> vaccum_cleaner):vc(vaccum_cleaner),h(house){};
	int dirtLevel() const override{return h->getDirtLevel(vc->getCurrentLoc());};

};
#endif