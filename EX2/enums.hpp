#ifndef ROBOT_ENUMS_H_
#define ROBOT_ENUMS_H_

enum class Direction { North, East, South, West, Stay, Finish };

enum class FileReadError {
  // TODO: Add more codes as we go
  Invalid = -100, // Generic error
  InvalidName,    // A=b incorrect variable name (A)
  InvalidValue,   // A=b incorrect value
  InvalidFormat   // File format incorrect/missing fields/lines
};

enum class AlgoState {
  CHARGING,
  TO_DOCK,
  TO_POS,
  WORKING,
  FINISH,
  EXPLORE,
  CLEANING

};

enum class LocType {
  Wall = -1,
  Dock = -2,
};

#define MAX_DIRT 9

#endif