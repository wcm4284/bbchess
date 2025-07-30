#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED


#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <functional>

#include "search.h"

namespace Engine {

class Thread {

    
    public:
    

        Thread(Search::SharedState&); 
        ~Thread();

        void start_searching();
        void run_custom_job(std::function<void()>); 
        void wait_for_job_finished();

        std::unique_ptr<Search::Worker> worker;

    private:

        void idle();


        std::function<void()>   job;

        std::mutex              mtx;
        std::condition_variable cv;
        std::thread             th;
        
        bool                    exit = false, searching = false;

};

class ThreadPool {

    public:

        ThreadPool(Search::SharedState, size_t); 
    

        void clear();
        void start_searching();
        void set(Search::SearchLimits&, Position&);
        uint64_t perft(std::string);


    private:

        void wait_for_all_threads();

        std::vector<std::unique_ptr<Thread>> threads;

        uint64_t accumulate(uint64_t Search::Worker::* member) {
    
            uint64_t total = 0;

            for (auto& th : threads)
                total += th->worker.get()->*member;

            return total;

        }

};

} // namespace Engine

#endif
