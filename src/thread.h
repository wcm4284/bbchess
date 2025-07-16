#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

#include "search.h"

namespace Engine {

class Thread {

	
	public:
		Thread();
		~Thread() {
			assert(th.joinable());
			th.join(); }


	private:

		void idle();

		std::unique_ptr<Search::Worker> worker;

		std::mutex              mtx;
		std::condition_variable cv;
		std::thread				th;


};

class ThreadPool {

	public:

	ThreadPool(size_t); 


	private:

		std::vector<std::unique_ptr<Thread>> threads;

};

} // namespace Engine
