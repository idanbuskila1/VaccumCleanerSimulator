#ifndef ROBOT_ENUMS_H_
#define ROBOT_ENUMS_H_

enum class Direction { North, East, South, West };
enum class Step { North, East, South, West, Stay, Finish };

enum class AlgoState {
  CHARGING,
  TO_DOCK,
  TO_POS,
  WORKING,
  FINISH,
  EXPLORE,
  CLEANING

};

#define MAX_DIRT 9

#endif