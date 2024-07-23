
#include "AlgoUtils.hpp"


inline pair<int,int> toPair(Direction dir, bool invert) {
  int multiplier = invert ? -1 : 1;
  switch (dir) {
  case Direction::North:
    return {-1 * multiplier, 0};
  case Direction::South:
    return {1 * multiplier, 0};
  case Direction::East:
    return {0, 1 * multiplier};
  case Direction::West:
    return {0, -1 * multiplier};
  default:
    return {0, 0};
  }
}

inline Direction toDir(int x, int y) {
  if (x == 1)
    return Direction::South;
  if (x == -1)
    return Direction::North;
  if (y == 1)
    return Direction::East;
  if (y == -1)
    return Direction::West;
  // default error value
  return Direction::North;
}

pair<int,int> getPosition(pair<int,int> pos, Direction dir) {
  auto direction = toPair(dir);
  return {pos.first + direction.first, pos.second + direction.second};
}

Direction getDirection(pair<int,int> src, pair<int,int> dst) {
  pair<int,int> diff = {dst.first - src.first, dst.second - src.second};
  if ((std::abs(diff.first) + std::abs(diff.second)) != 1) {
    std::cout << __FUNCTION__ << "ERROR!! invalid parameters in getDirection"
              << std::endl;
  }
  return toDir(diff.first, diff.second);
}

std::vector<Direction> dirPriority() {
  return {Direction::North, Direction::South, Direction::East, Direction::West};
}