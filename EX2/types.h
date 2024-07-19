#pragma once
#include "enums.hpp"

#include <iostream>

#define Pos std::pair<int, int>

enum class AlgoState {
  CHARGING,
  TO_DOCK,
  TO_POS,
  WORKING,
  FINISH,
  EXPLORE,
  CLEANING

};
