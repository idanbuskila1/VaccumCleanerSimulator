#include "index.hpp"
#include <iostream>
#include <fstream>
#include <dlfcn.h>
#include <dirent.h>

using std::cerr, std::endl, std::make_unique, std::unique_ptr,std::make_shared ,std::vector;
void parseArguments(int argc, char *argv[], std::string &housePath, std::string &algoPath, bool &isSummaryOnly, int &numThreads) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find("-house_path=") == 0) {
            housePath = arg.substr(12); // Length of "-house_path="
        } else if (arg.find("-algo_path=") == 0) {
            algoPath = arg.substr(11); // Length of "-algo_path="
        }
        else if(arg=="-summary_only"){
            isSummaryOnly=true;
        }
        else if (arg.find("-num_threads=") == 0) {
            std::string numThreadsStr = arg.substr(13); // Length of "-num_threads="
            numThreads = std::stoi(numThreadsStr);
        }
    }
}

void conductAllSimulations(int numThreads, std::string housePath, std::string algoPath, bool isSummaryOnly) {
    std::vector<std::thread> threadPool;
    SimulationManager manager;
    manager.openAlgorithms(algoPath);
    manager.initializeHouses(housePath);
    manager.setIsSummaryOnly(isSummaryOnly);
       // Start threads
    for (int i = 0; i < numThreads; ++i) {
        threadPool.emplace_back([&manager] {
            while(!manager.isSimulationDone()){
                manager.operateSimulations();
            }
        });
    }
    for (auto &thread : threadPool) {
        thread.join();
    }
    manager.makeSummary();
    manager.closeAlgorithms();
}
int main(int argc, char *argv[]){ 
    //parse arguments
    std::string housePath=".";
    std::string algoPath=".";
    bool isSummaryOnly=false;
    int numThreads=10;
    parseArguments(argc, argv, housePath, algoPath, isSummaryOnly, numThreads);
    conductAllSimulations(numThreads, housePath, algoPath, isSummaryOnly);
}