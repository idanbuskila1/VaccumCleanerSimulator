#ifndef SIMULATOR
#define SIMULATOR

#include "House.hpp"
#include "VaccumCleaner.hpp"
#include "abstract_algorithm.hpp"

using std::string ,std::shared_ptr, std::unique_ptr,std::make_shared, std::to_string;

class Simulator: public std::enable_shared_from_this<Simulator>{
    unique_ptr<House> h;
    unique_ptr<VaccumCleaner> vc;
    unique_ptr<AbstractAlgorithm> alg;
    size_t maxSteps;
    vector<string> StepLog; // Vector to store steps log
    string stepDescriptor; // string for the output file


    public:
    Simulator():h(nullptr),vc(nullptr),alg(nullptr){};
    int readHouseFile(const string& filename);
    void setAlgorithm(unique_ptr<AbstractAlgorithm> algorithm);
    size_t getBattery(){return vc->getBatterySteps();};
    bool isWall(Direction d){return h->isWallInDirection(d,vc->getCurrentLoc());};
    int getDirt(){return h->getDirtLevel(vc->getCurrentLoc());};
    void run();
    void makeOutputFile();
};
#endif