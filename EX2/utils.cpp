#include "utils.h"

string directionToString(Direction direction) {
    switch (direction) {
        case Direction::North: return "North";
        case Direction::East: return "East";
        case Direction::South: return "South";
        case Direction::West: return "West";
        default: return "Unknown";
    }
}

string stepToString(Step step) {
    switch (step) {
        case Step::North: return "North";
        case Step::East: return "East";
        case Step::South: return "South";
        case Step::West: return "West";
        case Step::Stay: return "Stay";
        case Step::Finish: return "Finish";
        default: return "Unknown";
    }
}

char stepToChar(Step step) {
    switch (step) {
        case Step::North: return 'N';
        case Step::East: return 'E';
        case Step::South: return 'S';
        case Step::West: return 'W';
        case Step::Stay: return 's';
        case Step::Finish: return 'F';
        default: return ' ';
    }
}

pair<int,int> calcNewPosition(Direction d, pair<int,int> pos) {
    pair<int,int> ret = pos;
    switch (d)
    {
        case Direction::North:
            ret.first = ret.first-1;
            break;
        case Direction::South:
            ret.first = ret.first+1;
            break;
        case Direction::East:
            ret.second = ret.second+1;
            break;
        case Direction::West:
            ret.second = ret.second-1;
            break;    
        default:
            break;
    }
    return ret;
}
