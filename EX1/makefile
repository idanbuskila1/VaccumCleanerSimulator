# Compiler
CXX = clang++

# Compiler flags
CXXFLAGS = -Wall -Wextra  -fsanitize=address -pedantic -std=c++14 -g

# Source files
SRCS = index.cpp House.cpp VaccumCleaner.cpp

# Header files
HDRS = index.h House.h VaccumCleaner.h

# Object files
OBJS = $(SRCS:.cpp=.o)

# Target executable
TARGET = out

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
