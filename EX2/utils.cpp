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

string stepToString(Direction step) {
    switch (step) {
        case Direction::North: return "North";
        case Direction::East: return "East";
        case Direction::South: return "South";
        case Direction::West: return "West";
        case Direction::Stay: return "Stay";
        case Direction::Finish: return "Finish";
        default: return "Unknown";
    }
}

char stepToChar(Direction step) {
    switch (step) {
        case Direction::North: return 'N';
        case Direction::East: return 'E';
        case Direction::South: return 'S';
        case Direction::West: return 'W';
        case Direction::Stay: return 's';
        case Direction::Finish: return 'F';
        default: return ' ';
    }
}

pair<int,int> calcNewPosition(Direction s, pair<int,int> pos) {
    pair<int,int> toCheck = pos;
    switch (s)
    {
        case Direction::North:
            toCheck.first = toCheck.first-1;
            break;
        case Direction::South:
            toCheck.first = toCheck.first+1;
            break;
        case Direction::East:
            toCheck.second = toCheck.second+1;
            break;
        case Direction::West:
            toCheck.second = toCheck.second-1;
            break;    
        default:
            break;
    }
    return toCheck;
}
