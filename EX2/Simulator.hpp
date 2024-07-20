#ifndef SIMULATOR
#define SIMULATOR

#include "House.hpp"
#include "VaccumCleaner.hpp"
#include "abstract_algorithm.h"
#include "WallsSensorObject.hpp"
#include "DirtSensorObject.hpp"
#include "BatteryMeterObject.hpp"


using std::string ,std::shared_ptr, std::unique_ptr,std::make_shared, std::to_string;

class Simulator: public std::enable_shared_from_this<Simulator>{
    shared_ptr<House> h;
    shared_ptr<VaccumCleaner> vc;
    unique_ptr<AbstractAlgorithm> alg;
    size_t maxSteps;
    vector<string> StepLog; // Vector to store steps log
    string stepDescriptor; // string for the output file
    WallsSensorObject wso;
    DirtSensorObject dso;
    BatteryMeterObject bmo;


    public:
    Simulator():h(nullptr),vc(nullptr),alg(nullptr),stepDescriptor(""),wso(h,vc),dso(h,vc),bmo(vc){};
    int readHouseFile(const string& filename);
    void setAlgorithm(unique_ptr<AbstractAlgorithm> algorithm);
    void run();
    void makeOutputFile();
};
#endif