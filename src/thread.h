/**
 * @file thread.h
 * @brief Contains classes used for multithreading
*/

#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <functional>

#include "search.h"

namespace Engine {

/**
 * @class Thread
 * @brief Contains multithreading logic, each thread owns one Thread object
*/
class Thread {
    public:
        Thread(Search::SharedState&); 
        ~Thread();

        /// @brief Tells this thread to start a search
        /// @note This method should only be called after the necessary setup has been performed
        void start_searching();

        /// @brief Tell this thread to run a job
        /// @param job The job (function) that you want this thread to run
        void run_custom_job(std::function<void()> job); 

        /// @brief Used by another thread to wait for this one to finish it's drop (stop searching, generally)
        void wait_for_job_finished();

        std::unique_ptr<Search::Worker> worker; ///< Worker object used for performing searches

    private:
        void idle();

        std::function<void()>   job; ///< The function that we want the thread to run

        std::mutex              mtx; 
        std::condition_variable cv;
        std::thread             th;
        
        bool                    exit = false, searching = false;
};

/**
 * @class ThreadPool
 * @brief Used to contain and control all threads
*/
class ThreadPool {
    public:
        ThreadPool(Search::SharedState, size_t); 
    
        /// @brief Clear information from the workers associated with each thread
        void clear();

        /// @brief Start a bestmove search
        void start_searching();
        
        /**
         * @brief Set the parameters of a search
         * @param limits Limits related to the search
         * @param pos Position to perform the search with
        */
        void set(Search::SearchLimits& limits, Position& pos);

        /**
         * @brief Run perft search
         * @param fen Position to run search on
         * @return The total number of nodes found at the searched position
         * @note The depth of the search is set through the set() function
        */
        uint64_t perft(std::string fen);

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
