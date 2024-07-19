# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -fPIC -Wall -Wextra -ICommon -ISimulation -IAlgorithm -g
LDFLAGS = -shared

# Sanitizer flags
SANITIZER_FLAGS = -fsanitize=address -fno-omit-frame-pointer

# Include directories
COMMON_INCLUDES = \
	enums.hpp \
	abstract_algorithm.h \
	battery_meter.h \
	dirt_sensor.h \
	wall_sensor.h

SIM_INCLUDES = \
	BatteryMeterObject.h \
	WallsSensorObject.h \
	DirtSensorObject.h \
	House.h \
	Utils.h \
	Simulator.h \
	RobotState.h

ALGO_INCLUDES = \
	MyAlgorithm.h \
	types.h \
	AlgoUtils.h \
	HouseManager.h

# Source files
SIM_SOURCES = \
	BatteryMeterObject.cpp \
	WallsSensorObject.cpp \
	DirtSensorObject.cpp \
	House.cpp \
	Simulator.cpp \
	RobotState.cpp \
	Utils.cpp

ALGO_SOURCES = \
	MyAlgorithm.cpp \
	HouseManager.cpp \
	AlgoUtils.cpp

# Object files
SIM_OBJECTS = $(SIM_SOURCES:.cpp=.o)
ALGO_OBJECTS = $(ALGO_SOURCES:.cpp=.o)
MAIN_OBJECT = index.o

# Targets
TARGET = EX2
LIBRARY = libsim.so

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(MAIN_OBJECT) $(ALGO_OBJECTS) $(LIBRARY)
	$(CXX) $(CXXFLAGS) $(SANITIZER_FLAGS) -o $@ $(MAIN_OBJECT) $(ALGO_OBJECTS) -L. -lsim -Wl,-rpath,.

$(LIBRARY): $(SIM_OBJECTS) $(ALGO_OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SANITIZER_FLAGS) -o $@ $^

%.o: %.cpp $(COMMON_INCLUDES) $(SIM_INCLUDES) $(ALGO_INCLUDES)
	$(CXX) $(CXXFLAGS) $(SANITIZER_FLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(LIBRARY) $(MAIN_OBJECT) $(SIM_OBJECTS) $(ALGO_OBJECTS)