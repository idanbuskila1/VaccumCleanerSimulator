#ifndef SIMULATOR
#define SIMULATOR

#include "abstractClasses.h"
#include "Algorithm.h"
using std::string;

class Simulator{
    shared_ptr<House> h;
    shared_ptr<VaccumCleaner> vc;
    unique_ptr<Algorithm> alg;
    size_t maxSteps;

    public:
    Simulator():h(nullptr),vc(nullptr),alg(nullptr){};
    int readHouseFile(const string& filename);
    void setAlgorithm(unique_ptr<Algorithm> algorithm);
    void run();
};
#endif