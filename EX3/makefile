.PHONY: all clean alg1

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
	@gcc -std=c++17 -g -o myrobot *.cpp simulator/*.cpp -I common -I algorithm -I simulator

run: 
	@echo "Build executables for all algorithms..."
	$(MAKE) compile
	@echo "Run the simulator..."
	@./myrobot -house_path=houses -algo_path=build/soFiles

# Target to build the algorithm
alg1:
	@echo "Creating build directory if it does not exist..."
	@mkdir -p build
	@echo "Running CMake..."
	@cd build && cmake -DALGORITHM_NAME=algorithem1 -DALGORITHM_SOURCES="208150953_A.cpp;208150953_A.hpp" -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" ..
	@echo "Building algorithm..."
	@cd build && $(MAKE)

alg2:
	@echo "Creating build directory if it does not exist..."
	@mkdir -p build
	@echo "Running CMake..."
	@cd build && cmake -DALGORITHM_NAME=algorithem2 -DALGORITHM_SOURCES="208150953_B.cpp;208150953_B.hpp" -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" ..
	@echo "Building algorithm..."
	@cd build && $(MAKE)



# Clean target to remove build artifacts
clean:
	@rm -rf build
