#ifndef WALLS_SENSOR
#define WALLS_SENSOR
#include "index.hpp"
#include "House.hpp"
#include "VaccumCleaner.hpp"

using std::shared_ptr;

class WallsSensor {
public:
	virtual ~WallsSensor() {}
	virtual bool isWall(Direction d) const = 0;
};


class WallsSensorObject: public WallsSensor{
    shared_ptr<House> h;
    shared_ptr<VaccumCleaner> vc;
public:
    WallsSensorObject(shared_ptr<House> house,shared_ptr<VaccumCleaner> vaccumCleaner):h(house),vc(vaccumCleaner){};
    ~WallsSensorObject() =default;
	bool isWall(Direction d) const override{return h->isWallInDirection(d,vc->getCurrentLoc());};
};
#endif