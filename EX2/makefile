# Compiler
CXX = g++

# Compiler flags
CXXFLAGS =-fsanitize=address -g -Wall -Wextra -Werror -pedantic -std=c++20 

# Source files
SRCS = index.cpp House.cpp VaccumCleaner.cpp Simulator.cpp MyAlgorithm.cpp utils.cpp HouseManager.cpp

# Header files
HDRS = abstract_algorithm.h wall_sensor.h battery_meter.h dirt_sensor.h enums.h utils.h index.hpp House.hpp VaccumCleaner.hpp Simulator.hpp BatteryMeterObject.hpp HouseManager.h DirtSensorObject.hpp WallsSensorObject.hpp MyAlgorithm.hpp 

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