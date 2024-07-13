#ifndef WALLS_SENSOR
#define WALLS_SENSOR

#include "wall_sensor.hpp"
#include "enums.hpp"
#include "Simulator.hpp"

using std::shared_ptr;

class WallsSensorObject: public WallsSensor{
    shared_ptr<Simulator> sim;
public:
    WallsSensorObject(shared_ptr<Simulator> simulator):sim(simulator){};
	bool isWall(Direction d) const override{return sim->isWall(d);};
};
#endif