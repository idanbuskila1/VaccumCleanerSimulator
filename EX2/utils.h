#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <set>
#include <map>
#include <queue>
#include "enums.hpp"

using std::string, std::vector, std::pair, std::queue, std::map, std::set; 
using std::cout, std::endl, std::ofstream;

const int WALL = 999;

string directionToString(Direction direction);
string stepToString(Direction step);
char stepToChar(Direction step);
pair<int,int> calcNewPosition(Direction s, pair<int,int> pos);
#endif //UTILS_H