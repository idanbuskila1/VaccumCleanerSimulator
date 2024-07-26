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

const pair<int,int> DOCK = {0,0};

string directionToString(Direction direction);
string stepToString(Step step);
char stepToChar(Step step);
pair<int,int> calcNewPosition(Direction d, pair<int,int> pos);
#endif //UTILS_H