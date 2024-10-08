#ifndef SIMULATOR
#define SIMULATOR

#include "House.hpp"
#include "VaccumCleaner.hpp"
#include "../common/abstract_algorithm.hpp"
#include "WallsSensorObject.hpp"
#include "DirtSensorObject.hpp"
#include "BatteryMeterObject.hpp"
#include "../common/structs.hpp"


using std::string ,std::shared_ptr, std::unique_ptr,std::make_shared, std::to_string;

class Simulator{
    shared_ptr<House> h;
    shared_ptr<VaccumCleaner> vc;
    unique_ptr<AbstractAlgorithm> alg;
    size_t maxSteps;
    vector<string> StepLog; // Vector to store steps log
    string stepDescriptor; // string for the output file
    WallsSensorObject wso;
    DirtSensorObject dso;
    BatteryMeterObject bmo;
    bool isRuntimeError = false;
    string status="WORKING";


    public:
    Simulator():h(nullptr),vc(nullptr),alg(nullptr),stepDescriptor(""),wso(h,vc),dso(h,vc),bmo(vc){};
    // read house file and create initial objects and data members
    int readHouseFile(const string& filename);
    // initialize simulation parameters from a house file data
    void setSimulationData(InputFileData data);
    //set the algorithm to be used
    void setAlgorithm(unique_ptr<AbstractAlgorithm> algorithm);
    //run the simulation
    void run();
    //calcualte score after the simulation.
    int calcScore();
    //make ouput file
    void makeOutputFile(string name);
    //make log file
    void makeLog(string name);
    //return if there was an error in the algorithm moves (move into wall etc.)
    bool getIsRuntimeError()const{return isRuntimeError;};
    //return the error message if there was an error
    string getErrorMessage()const{return StepLog.back();};
};
#endif