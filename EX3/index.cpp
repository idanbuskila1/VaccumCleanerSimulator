#include "index.hpp"
#include <iostream>
#include <fstream>
#include <dlfcn.h>
#include <dirent.h>

using std::cerr, std::endl, std::make_unique, std::unique_ptr,std::make_shared ,std::vector;
void closeEntries(vector<void *> vec){
    for(void* handle:vec){
        dlclose(handle);
        cout<<handle<<endl;
    }

}
int main(int argc, char *argv[]){ 
    //parse arguments
    std::string housePath="";
    std::string algoPath="";
    std::vector<void*> files_entries;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find("-house_path=") == 0) {
            housePath = arg.substr(12); // Length of "-house_path="
        } else if (arg.find("-algo_path=") == 0) {
            algoPath = arg.substr(11); // Length of "-algo_path="
            std::cout << algoPath << std::endl;
            std::cout << housePath << std::endl;
            // Open the directory
            DIR* dir = opendir(algoPath.c_str());
            if (dir) {
                // Read all the files in the directory
                struct dirent* entry;
                while ((entry = readdir(dir)) != nullptr) {
                    std::cout << entry << std::endl;
                    std::string filename = entry->d_name;
                    std::string filepath = algoPath + "/" + filename;
                    
                    // Check if the file is a shared object (.so)
                    if (filename.length() > 3 && filename.substr(filename.length() - 3) == ".so") {
                        // Open the shared object with dlopen
                        void* handle = dlopen(filepath.c_str(), RTLD_NOW);
                        if (handle) {
                            cout<<handle<<endl;
                            files_entries.push_back(handle);
                        } else {
                            std::cerr << "Failed to open shared object: " << dlerror() << std::endl;
                        }
                    }
                }
                
                // Close the directory
                closedir(dir);
            } else {
                std::cerr << "Failed to open directory: " << algoPath << std::endl;
            }
        }
    }
    SimulationManager manager;
    manager.initializeHouses(housePath);
    manager.operateSimulations();
    AlgorithmRegistrar::getAlgorithmRegistrar().clear();
    closeEntries(files_entries);
    }

/*Simulator simulator;
    int res = simulator.readHouseFile(argv[1]);
    if(res == -1) return 1;
	MyAlgorithm algo;
    simulator.setAlgorithm(algo);
	simulator.run();
    if(simulator.getIsRuntimeError()){
        cerr << "Error: " << simulator.getErrorMessage() << endl;
        return 1;
    } 
    simulator.makeOutputFile(argv[1]);
    simulator.makeLog(argv[1]);*/