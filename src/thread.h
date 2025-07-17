#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <functional>

#include "search.h"

namespace Engine {

class Thread {

	
	public:
		Thread();
		~Thread();

		void start_searching();
		void run_custom_job(std::function<void()>); 


		std::unique_ptr<Search::Worker> worker;

	private:

		void idle();
		void wait_for_job_finished();


		std::function<void()>   job;

		std::mutex              mtx;
		std::condition_variable cv;
		std::thread				th;
		
		bool                    exit = false, searching = false;

};

class ThreadPool {

	public:

	ThreadPool(size_t); 

	void start_searching();
	void set(Search::SearchLimits& limits);


	private:

		std::vector<std::unique_ptr<Thread>> threads;

};

} // namespace Engine
