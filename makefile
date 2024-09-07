.PHONY: all clean alg1
CXXFLAGS =-std=c++20 -Wall -Wextra -Werror -pedantic -pthread

# Default target
all: alg1

compile:
	@echo "Build Algorithem 1"
	$(MAKE) alg1
	@echo "Build Algorithem 2"
	$(MAKE) alg2
	@echo "Build Simulator"
	$(MAKE) sim


sim:
	@echo "Build Simulator"
	@g++ $(CXXFLAGS) -o myrobot simulator/*.cpp common/*.cpp -I common -I algorithm -I simulator

compile-run: 
	@echo "Build executables for all projects..."
	$(MAKE) compile
	$(MAKE) run
	

run:
	@echo "Run the simulator..."
#	@./myrobot
#	@./myrobot -house_path=houses
#	@./myrobot -house_path=houses -algo_path=build/soFiles -summary_only
	@./myrobot -algo_path=build/soFiles -house_path=houses  -num_threads=8

# Target to build the algorithm
alg1:
	@echo "Creating build directory if it does not exist..."
	@mkdir -p build
	@echo "Running CMake..."
	@cd build && cmake -DALGORITHM_NAME=208150953_A -DALGORITHM_SOURCES="208150953_A.cpp;208150953_A.hpp" -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" ..
	@echo "Building algorithm..."
	@cd build && $(MAKE)

alg2:
	@echo "Creating build directory if it does not exist..."
	@mkdir -p build
	@echo "Running CMake..."
	@cd build && cmake -DALGORITHM_NAME=208150953_B -DALGORITHM_SOURCES="208150953_B.cpp;208150953_B.hpp" -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" ..
	@echo "Building algorithm..."
	@cd build && $(MAKE)



# Clean target to remove build artifacts
clean:
	@rm -rf build
