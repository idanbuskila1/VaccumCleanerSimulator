#ifndef WALL_S
#define WALL_S

#include "enums.hpp"


class WallsSensor {
public:
	virtual ~WallsSensor() {}
	virtual bool isWall(Direction d) const = 0;
};

#endif  // WALL_S
