
#ifndef ABSTRACT
#define ABSTRACT
#include <cstddef>
#include "WallsSensorObject.h"
#include "DirtSensorObject.h"
#include "BatteryMeterObject.h"
#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
using std::shared_ptr ,std::make_shared, std::unique_ptr ,std::make_unique;;

enum class Direction { North, East, South, West };
enum class Step { North, East, South, West, Stay, Finish };
enum class HouseState{Dirt0=0,Dirt1=1,Dirt2=2,Dirt3=3,Dirt4=4,Dirt5=5,Dirt6=6,Dirt7=7,Dirt8=8,Dirt9=9,Wall='W',Docking='D',Empty=' '};//house tile states

class WallsSensor {
public:
	virtual ~WallsSensor() {}
	virtual bool isWall(Direction d) const = 0;
};

class DirtSensor {
public:
	virtual ~DirtSensor() {}
	virtual int dirtLevel() const = 0;
};

class BatteryMeter {
public:
	virtual ~BatteryMeter() {}
	virtual std::size_t getBatteryState() const = 0;
};

class AbstractAlgorithm {
public:
	virtual ~AbstractAlgorithm() {}
	virtual void setMaxSteps(std::size_t maxSteps) = 0;
	virtual void setWallsSensor(const WallsSensor&) = 0;
	virtual void setDirtSensor(const DirtSensor&) = 0;
	virtual void setBatteryMeter(const BatteryMeter&) = 0;
	virtual Step nextStep() = 0;
};
#endif

