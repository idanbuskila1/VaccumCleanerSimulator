#ifndef WALLS_SENSOR
#define WALLS_SENSOR
#include "abstractClasses.h"
#include "House.h"
#include "VaccumCleaner.h"
class WallsSensorObject: public WallsSensor{
    shared_ptr<House> h;
    shared_ptr<VaccumCleaner> vc;
public:
    WallsSensorObject(shared_ptr<House> house,shared_ptr<VaccumCleaner> vaccumCleaner):h(house),vc(vaccumCleaner){};
    ~WallsSensorObject() =default;
	bool isWall(Direction d) const override{return h->isWallInDirection(d,vc->getCurrentLoc());};
};
#endif