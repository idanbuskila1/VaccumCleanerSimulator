# Compiler
CXX = g++

# Compiler flags
CXXFLAGS =-fsanitize=address -g -Wall -Wextra -Werror -pedantic -std=c++20 

# Source files
SRCS = index.cpp House.cpp VaccumCleaner.cpp Simulator.cpp MyAlgorithm.cpp AlgoUtils.cpp HouseManager.cpp 

# Header files
HDRS = abstract_algorithm.hpp wall_sensor.hpp battery_meter.hpp dirt_sensor.hpp enums.hpp index.hpp House.hpp VaccumCleaner.hpp HouseManager.hpp Simulator.hpp BatteryMeterObject.hpp DirtSensorObject.hpp WallsSensorObject.hpp MyAlgorithm.hpp AlgoUtils.hpp 

# Object files
OBJS = $(SRCS:.cpp=.o)

# Target executable
TARGET = myrobot

# Build the executable
$(TARGET): $(OBJS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $<

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET) BatteryMeterObject.o DirtSensorObject.o WallsSensorObject.o

#Phony targets
.PHONY: all clean

# Default target
all: $(TARGET)