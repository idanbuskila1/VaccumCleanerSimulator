#ifndef WALLS_S_O
#define WALLS_S_O

#include "../common/wall_sensor.hpp"
#include "../common/enums.hpp"
#include "VaccumCleaner.hpp"
#include "House.hpp"

using std::shared_ptr;

class WallsSensorObject: public WallsSensor{
    shared_ptr<VaccumCleaner> vc;
    shared_ptr<House> h;
public:
    WallsSensorObject(shared_ptr<House> house, shared_ptr<VaccumCleaner> vaccum_cleaner):vc(vaccum_cleaner),h(house){};
	bool isWall(Direction d) const override{return h->isWallInDirection(d,vc->getCurrentLoc());};
};
#endif