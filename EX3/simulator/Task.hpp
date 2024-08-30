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
    boost::asio::io_context& ioContext;
    std::latch& work_done;
    // internal managed vars:
    std::atomic<bool> guard;
    std::thread my_thread;
    // task that doesn't finish on time shall got -1
    // otherwise, shall get the time in took to finish
    int result;
    shared_ptr<std::counting_semaphore<>> currentThreads;

public:
    static void timerHandler(const boost::system::error_code& ec, Task& task, time_point start, pthread_t thread_handler);
    Task(size_t index, Simulator& sim, int max_time, boost::asio::io_context& ioContext, std::latch& work_done,shared_ptr<std::counting_semaphore<>>& semaphore)
        : index(index), simulator(sim), max_time(max_time), ioContext(ioContext), work_done(work_done),result(0) {currentThreads=semaphore;};
    void run();
    int get_result() const;
    void join();
    void detach();
};