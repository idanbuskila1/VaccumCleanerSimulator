# Compiler
CXX = g++

# Compiler flags
CXXFLAGS =-fsanitize=address -Wall -Wextra -Werror -pedantic -std=c++20 

# Source files
SRCS = index.cpp House.cpp VaccumCleaner.cpp CleaningAlgorithm.cpp

# Header files
HDRS = index.h House.h VaccumCleaner.h CleaningAlgorithm.h

# Object files
OBJS = $(SRCS:.cpp=.o)

# Target executable
TARGET = myrobot

# Build the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $<

# Clean up build files
#clean:
#	rm -f $(OBJS) $(TARGET)

# Phony targets
#.PHONY: all clean

# Default target
all: $(TARGET)
