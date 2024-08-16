cmake_minimum_required(VERSION 3.12)
project(YourProjectName)

set(CMAKE_CXX_STANDARD 17)

# Allow the user to specify the algorithm folder and source files via CMake variables
set(ALGORITHM_NAME "" CACHE STRING "Name of the algorithm folder (e.g., algorithem1)")
set(ALGORITHM_SOURCES "" CACHE STRING "List of source files in the algorithm folder (e.g., 208150953_A.cpp;208150953_A.hpp)")

if(ALGORITHM_NAME STREQUAL "" OR ALGORITHM_SOURCES STREQUAL "")
    message(FATAL_ERROR "Please specify the ALGORITHM_NAME and ALGORITHM_SOURCES")
endif()

# Convert the list of source files into a path within the algorithm folder
list(TRANSFORM ALGORITHM_SOURCES PREPEND "${ALGORITHM_NAME}/")

# Add the specified algorithm directory as a library
add_library(${ALGORITHM_NAME} SHARED ${ALGORITHM_SOURCES})

# Force the architecture to ARM64 for Apple Silicon (M1)
if(APPLE AND CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
    set(CMAKE_OSX_ARCHITECTURES "arm64")
endif()

# Add the NOT_IMPLEMENTED_FLAG
add_definitions(-DNOT_IMPLEMENTED_FLAG)

# Include the common directory
target_include_directories(${ALGORITHM_NAME} PUBLIC common)

# Allow undefined symbols to be resolved later
set_target_properties(${ALGORITHM_NAME} PROPERTIES
    LINK_FLAGS "-undefined dynamic_lookup"
    LINKER_LANGUAGE CXX
    OUTPUT_NAME ${ALGORITHM_NAME}
    SUFFIX ".so"
)

# Output the shared object file to the directory "soFiles"
set_target_properties(${ALGORITHM_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/soFiles"
)

# Optional: Enable exports for shared library
set_target_properties(${ALGORITHM_NAME} PROPERTIES ENABLE_EXPORTS 1)
