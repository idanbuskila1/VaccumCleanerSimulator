#include "index.hpp"
using std::cerr, std::endl, std::make_unique, std::unique_ptr,std::make_shared;

int main(int argc, char *argv[]){ 
    if(argc!=2 || !argv[1]){
        cerr << "Error:must pass exactly 1 argument of input file name." << endl;
        return 1;
    }
    Simulator simulator;
    int res = simulator.readHouseFile(argv[1]);
    if(res == -1) return 1;
	unique_ptr<AbstractAlgorithm> algo = make_unique<Algorithm>();
    simulator.setAlgorithm(std::move(algo));
	simulator.run();
    simulator.makeOutputFile();
}