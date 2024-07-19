#include "index.hpp"
using std::cerr, std::endl, std::make_unique, std::unique_ptr,std::make_shared;

int main(int argc, char *argv[]){ 
    if(argc!=2 || !argv[1]){
        cerr << "Error:must pass exactly 1 argument of input file name." << endl;
        return 1;
    }
    Simulator simulator;
    //  TODO: Handle empty command line args etc.
    if (simulator.readHouseFile(argv[1]) < 0)
        std::cout << "File read error. Stopping Simulator" << std::endl;
    MyAlgorithm algo;
    simulator.setAlgorithm(algo);
    simulator.run();
    simulator.dump("output.txt");
}