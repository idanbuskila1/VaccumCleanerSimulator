#ifndef TASK
#define TASK

#include <thread>
#include <pthread.h>
#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
#include <list>
#include <latch>
#include <vector>
#include "Simulator.hpp"
#include <semaphore>

using TIME_UNIT = std::chrono::milliseconds;
using time_point = std::chrono::time_point<std::chrono::system_clock>;

class Task {
    const size_t index;
    Simulator& simulator;
    const int max_time;
    std::shared_ptr<boost::asio::io_context> ioContext;
    std::latch& work_done;
    // internal managed vars:
    std::atomic<bool> guard;
    std::thread my_thread;
    // task that doesn't finish on time shall got -1
    // otherwise, shall get 0
    int result;
    shared_ptr<std::counting_semaphore<>> currentThreads;

public:
    static void timerHandler(const boost::system::error_code& ec, Task& task, time_point start, pthread_t thread_handler);
    Task(size_t index, Simulator& sim, int max_time, std::shared_ptr<boost::asio::io_context> ioContext, std::latch& work_done,shared_ptr<std::counting_semaphore<>>& semaphore)
        : index(index), simulator(sim), max_time(max_time), ioContext(ioContext), work_done(work_done),result(0) {currentThreads=semaphore;};
    //run the task: call the simulator.run() and wait for it to finish
    void run();
    //return the result of the task. -1 if the task didn't finish on time, 0 otherwise
    int get_result() const;
    //join the task
    void join();
    //detach the task
    void detach();
};
#endif // TASK