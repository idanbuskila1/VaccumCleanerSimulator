#include "index.hpp"
#include <iostream>
#include <fstream>
#include <dlfcn.h>
#include <dirent.h>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind/bind.hpp>
#include <semaphore>
#include "simulator/Task.hpp"


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

void run_tasks(boost::asio::io_context& ioContext,int numThreads, std::string housePath, std::string algoPath, bool isSummaryOnly,unique_ptr<SimulationManager>& manager) {
    using namespace std::chrono_literals;

    auto start = std::chrono::system_clock::now();
    std::vector<std::thread> threadPool;
    boost::asio::io_context io;
    manager->openAlgorithms(algoPath);
    manager->initializeHouses(housePath);
    manager->setIsSummaryOnly(isSummaryOnly);
    unique_ptr<std::vector<Simulator>> simulators = make_unique<std::vector<Simulator>>(manager->prepareAllSimulations());
    const long num_tasks = long(simulators->size());
    std::latch work_done(num_tasks);
    shared_ptr<std::counting_semaphore<>> currentThreads= make_shared<std::counting_semaphore<>>(numThreads);
    

    // initiate tasks
    std::list<Task> tasks;
    size_t task_index = 0;
    for(int i=0;i<num_tasks;i++){ 
        //manager.getTimeLimit(i)
        tasks.emplace_back(task_index, (*simulators)[i], 3, ioContext, work_done,currentThreads);
        ++task_index;
    }

    task_index = 0;
    for(auto& task: tasks) {
        currentThreads->acquire();
        auto now = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<TIME_UNIT>(now - start);
        std::cout << "Starting task " << task_index << " at time: " << duration.count() << std::endl;
        ++task_index;
        task.run();
    }
    // wait on latch, for all threads to report done
    work_done.wait();

    std::vector<int> results;
    for(auto& task: tasks) {
        results.push_back(task.get_result());
        task.join();
    }
    manager->sumerrizeAllSimulations(*simulators,results);
    manager->makeSummary();
}

int main(int argc, char *argv[]){
    //parse arguments
    std::string housePath=".";
    std::string algoPath=".";
    bool isSummaryOnly=false;
    int numThreads=10;
    parseArguments(argc, argv, housePath, algoPath, isSummaryOnly, numThreads);
    auto ioContext = std::make_shared<boost::asio::io_context>();
    auto manager = make_unique<SimulationManager>();
    auto start = std::chrono::system_clock::now();
    
    // work guard to keep io_context running till stopped
    // even if there is no active timer to wait for
    auto workGuard = boost::asio::make_work_guard(*ioContext);
    // Run io_context in a separate thread
    std::thread ioThread([ioContext]() {
        ioContext->run();
    });
    run_tasks(*ioContext,numThreads,housePath,algoPath,isSummaryOnly,manager);

    // stop the io_context, and join the io_thread
    ioContext->stop();
    if(ioThread.joinable()) {
        ioThread.join();
    }

    
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<TIME_UNIT>(now - start);
    // print duration that passed
    std::cout << "Total runtime: " << duration.count() << std::endl;
    // conductAllSimulations(numThreads, housePath, algoPath, isSummaryOnly, manager);
    manager->closeAlgorithms();
}

// void conductAllSimulations(int numThreads, std::string housePath, std::string algoPath, bool isSummaryOnly,SimulationManager& manager) {
//     std::vector<std::thread> threadPool;
//     boost::asio::io_context io;
//     manager.openAlgorithms(algoPath);
//     manager.initializeHouses(housePath);
//     manager.setIsSummaryOnly(isSummaryOnly);
//     std::vector<Simulator> simulators = manager.prepareAllSimulations();
//     std::atomic<size_t> simulationsLeft{simulators.size()};


//        // Start threads
//     for (int i = 0; i < numThreads; ++i) {
//         threadPool.emplace_back([&manager,&io,&simulators] {
//             while(!manager.isSimulationDone()){
//                 int curSim = manager.getSimulationNumber();
//                 if(curSim==-1){
//                     break;
//                 }
//                 boost::asio::steady_timer t(io, boost::asio::chrono::milliseconds(20000));
//                 t.async_wait([&](const boost::system::error_code& error) {
//     killThread(error, &t,curSim);
// });
//                 std::cout<<"simulation number: "<<curSim<<std::endl;
//                 simulators[curSim].run();
//                 t.cancel();
//             }
//         });
//     }
//     while(!manager.isSimulationDone()){
//         io.restart();
//         io.run_one();
//     }
//     for (auto &thread : threadPool) {
//         thread.join();
//     }
//     manager.sumerrizeAllSimulations(simulators);
//     manager.makeSummary();
// }