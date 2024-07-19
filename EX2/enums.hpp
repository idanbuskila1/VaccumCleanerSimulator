#ifndef ROBOT_ENUMS_H_
#define ROBOT_ENUMS_H_

enum class Direction { North, East, South, West };
enum class Step { North, East, South, West, Stay, Finish };

#define MAX_DIRT 9
enum class LocType {
  Wall = -1,
  Dock = 100,
};


#endif  // ROBOT_ENUMS_H_
