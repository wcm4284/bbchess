#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

namespace Engine {

class Thread {

	
	public:
		Thread();



	private:
		std::mutex              mtx;
		std::condition_variable cv;
		std::thread				th;


};

class ThreadPool {

	public:


	private:

		std::vector<Thread> threads;

};

} // namespace Engine
