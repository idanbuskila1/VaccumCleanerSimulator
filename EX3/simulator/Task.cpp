#include "Task.hpp"
// private function for the timer handler
void Task::timerHandler(const boost::system::error_code& ec, Task& task, time_point start, pthread_t thread_handler) {
        if (ec == boost::asio::error::operation_aborted) {
            std::cout << "Timer for task " << task.index << " was canceled" << std::endl;
        } else if (!ec) {
            auto now = std::chrono::system_clock::now();
            auto duration = std::chrono::duration_cast<TIME_UNIT>(now - start);
            // print duration that passed
            std::cout << "Timer for task " << task.index << " expired after " << duration.count() << std::endl;
            bool expected = false;
            bool success = task.guard.compare_exchange_strong(expected, true);
            if(!success) {
                // the task already wrote
                std::cout << "Task " << task.index << ", already wrote, before timer expiration" << std::endl;
            }
            else {
                task.result = -1;
                // send a cancel request to the thread that seems to be stuck
                pthread_cancel(thread_handler);
                task.work_done.count_down();
                task.currentThreads->release();
            }
        }
    }
    void Task::run() {
        my_thread = std::thread([this] {
            // allow this thread to be canceled asyncronously from another thread
            pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);
            pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, nullptr);            
            // set the timer
            boost::asio::steady_timer timer(ioContext, TIME_UNIT(max_time));
            auto curr_time = std::chrono::system_clock::now();
            auto thread_handler = pthread_self();
            timer.async_wait([&](const boost::system::error_code& ec) {
                timerHandler(ec, *this, curr_time, thread_handler);
            });
            //run the simulator
            simulator.run();

            // now we are after the task was done, or there was a timeout
            timer.cancel();
            // there still might be a race condition, this is why we have the atomic bool
            auto end_time = std::chrono::system_clock::now();
            bool expected = false;
            bool success = guard.compare_exchange_strong(expected, true);
            auto duration = std::chrono::duration_cast<TIME_UNIT>(end_time - curr_time);
            if(!success) {
                // the timer expired and wrote
                std::cout << "Task " << index << ", timer already expired and wrote" << std::endl;
            }
            else {
                std::cout << "Task" << index << " Finished in "<<duration.count() << std::endl;
                work_done.count_down();
                currentThreads->release();
            }
        });
    }
    int Task::get_result() const {
        return result;
    }
    void Task::join() {
        if(my_thread.joinable()) {
            my_thread.join();
        }
    }
    void Task::detach() {
        if(my_thread.joinable()) {
            my_thread.detach();
        }
    }

