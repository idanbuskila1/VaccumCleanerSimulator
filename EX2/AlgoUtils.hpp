#pragma once

#include "enums.hpp"

#include <cstdlib>
#include <utility>
#include <vector>
#include <iostream>


using std::pair;

constexpr pair<int,int> DOCK_POS = {0,0};
constexpr int BATTERY_BUFF = 2; // can be percentage

inline pair<int,int> toPair(Direction dir, bool invert = false);

inline Direction toDir(int x, int y);

pair<int,int> getPosition(pair<int,int> pos, Direction dir);

Direction nextDir(const Direction &dir);

std::vector<Direction> dirPriority();

Direction getDirection(pair<int,int> src, pair<int,int> dst);

std::ostream &operator<<(std::ostream &out, const AlgoState &Direction);
