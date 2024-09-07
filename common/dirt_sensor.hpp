#ifndef DIRT_S
#define DIRT_S

class DirtSensor {
public:
	virtual ~DirtSensor() {}
	virtual int dirtLevel() const = 0;
};

#endif
