#include "index.hpp"
#include <iostream>
#include <fstream>
#include <dlfcn.h>
#include <dirent.h>
#include <boost/asio.hpp>



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
void killThread(){
    std::cout<<"Thread is killed"<<std::endl;
}
void conductAllSimulations(int numThreads, std::string housePath, std::string algoPath, bool isSummaryOnly) {
    std::vector<std::thread> threadPool;
    std::vector<Simulator> simulators;
    SimulationManager manager;
    boost::asio::io_context io;

    manager.openAlgorithms(algoPath);
    manager.initializeHouses(housePath);
    manager.setIsSummaryOnly(isSummaryOnly);

       // Start threads
    for (int i = 0; i < numThreads; ++i) {
        threadPool.emplace_back([&manager,&io] {
            while(!manager.isSimulationDone()){
                





                boost::asio::steady_timer t(io, boost::asio::chrono::seconds(4));
                t.async_wait(&killThread);
                io.restart();
                manager.operateSimulations();
                t.cancel();
            }
        });
    }
    for (auto &thread : threadPool) {
        thread.join();
    }
    manager.makeSummary();
    manager.closeAlgorithms();
}

void print(const boost::system::error_code& /*e*/)
{
  std::cout << "Hello, world!" << std::endl;
}


int main(int argc, char *argv[]){ 
    boost::asio::io_context io;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(4));
    t.async_wait(&print);
    boost::asio::steady_timer t2(io, boost::asio::chrono::seconds(6));
    t2.async_wait(&print);
    io.run_one();
    io.run_one();

    io.restart();
    boost::asio::steady_timer t3(io, boost::asio::chrono::seconds(8));
    t3.async_wait(&print);
    io.run();

    //parse arguments
    std::string housePath=".";
    std::string algoPath=".";
    bool isSummaryOnly=false;
    int numThreads=10;
    parseArguments(argc, argv, housePath, algoPath, isSummaryOnly, numThreads);
    // conductAllSimulations(numThreads, housePath, algoPath, isSummaryOnly);
}