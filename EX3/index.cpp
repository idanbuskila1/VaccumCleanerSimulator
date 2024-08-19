#include "index.hpp"
#include <iostream>
#include <fstream>
#include <dlfcn.h>
#include <dirent.h>

using std::cerr, std::endl, std::make_unique, std::unique_ptr,std::make_shared ,std::vector;
void closeEntries(vector<void *> vec){
    for(void* handle:vec){
        dlclose(handle);
    }
}

void createErrFile(string errorFilePath, string msg){
    std::cerr << "Failed to open shared object." << std::endl;
    std::ofstream errorFile(errorFilePath);
    if (errorFile.is_open()) {
        errorFile << msg << std::endl;
        errorFile.close();
    } else {
        std::cerr << "Failed to create error file: " << errorFilePath << std::endl;
    }
}

void openAlgorithms(string path, std::vector<void*>& files_entries){
    // Open the directory
    if (path == ".") {
        path = std::filesystem::current_path().string();
    }
    DIR* dir = opendir(path.c_str());
    size_t prev_count=0;
    if (dir) {
        // Read all the files in the directory
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string filename = entry->d_name;
            std::string filepath = path + "/" + filename;
            // Check if the file is a shared object (.so)
            if (filename.length() > 3 && filename.substr(filename.length() - 3) == ".so") {
                // Open the shared object with dlopen
                cout<<"algo "<<entry->d_name<<endl;
                void* handle = dlopen(filepath.c_str(), RTLD_NOW);
                if (handle) {
                    files_entries.push_back(handle);
                    if(AlgorithmRegistrar::getAlgorithmRegistrar().count() != prev_count+1){
                        createErrFile(filename.substr(0, filename.length() - 3) + ".error", "couldnt registar the algorithm (count did not updated after dlopen).");
                    }
                    prev_count++;
                } else createErrFile(filename.substr(0, filename.length() - 3) + ".error", dlerror());
            }
        }      
        // Close the directory
        closedir(dir);
        cout<<AlgorithmRegistrar::getAlgorithmRegistrar().count()<<endl;
    } else {
        std::cerr << "Failed to open directory: " << path << std::endl;
    }
}

int main(int argc, char *argv[]){ 
    //parse arguments
    std::string housePath=".";
    std::string algoPath=".";
    bool isSummaryOnly=false;
    std::vector<void*> files_entries;

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
    }
    openAlgorithms(algoPath,files_entries);
    //manage simulation.
    SimulationManager manager;
    manager.initializeHouses(housePath);
    manager.operateSimulations(isSummaryOnly);
    manager.makeSummary();
    AlgorithmRegistrar::getAlgorithmRegistrar().clear();
    closeEntries(files_entries);
}