#ifndef DIRT_SENSOR
#define DIRT_SENSOR

#include <memory>

#include "House.hpp"
#include "VaccumCleaner.hpp"
#include "dirt_sensor.hpp"

class DirtSensorObject: public DirtSensor{
    std::shared_ptr<Simulator> sim;

public:
    DirtSensorObject(std::shared_ptr<Simulator> simulator):sim(simulator){};
	int dirtLevel() const override{return sim->getDirt();};

};
#endif