.PHONY: all clean alg1
CXXFLAGS =-fsanitize=address -Wall -Wextra -Werror -pedantic -std=c++20 

# Default target
all: alg1

compile:
	@echo "Build Algorithem 1"
	$(MAKE) alg1
	@echo "Build Algorithem 2"
	$(MAKE) alg2
	@echo "Build executables for all algorithms..."
	$(MAKE) build-exe

build-exe:
	@echo "Build executables for all algorithms..."
	@g++ $(CXXFLAGS) -g -o myrobot *.cpp simulator/*.cpp common/*.cpp -I common -I algorithm -I simulator

run: 
	@echo "Build executables for all algorithms..."
	$(MAKE) compile
	@echo "Run the simulator..."
#	@./myrobot
#	@./myrobot -algo_path=build/soFiles
#	@./myrobot -house_path=houses -algo_path=build/soFiles -summary_only
	@./myrobot -house_path=houses -algo_path=build/soFiles 

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
