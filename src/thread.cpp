#include <iostream>
#include <cassert>

#include "thread.h"

namespace Engine {

Thread::Thread() : th(&Thread::idle, this) {

	this->worker = std::make_unique<Search::Worker>();
}

Thread::~Thread() {
	exit = true;
	start_searching();
	th.join();
}

void Thread::run_custom_job(std::function<void()> f) {
	
	job = std::move(f);
	searching = true;
	cv.notify_one();
}

void Thread::wait_for_job_finished() {
	std::unique_lock<std::mutex> lock(mtx);
	cv.wait(lock, [&] { return !searching; });
	std::cout << "done waiting\n";
}

void Thread::start_searching() {
	assert(worker);
	run_custom_job([this] { worker->start_searching(); });
}

void Thread::idle() {

	while (true) {
		searching = false;	
		std::unique_lock<std::mutex> lock(mtx);
		cv.wait(lock, [&] { return searching; });
		
		if (exit)
			break;
		
		std::function<void()> work = std::move(job);
		job					   	   = nullptr;

		if (work)
			work();
	}

}


ThreadPool::ThreadPool(size_t requested) {

	assert(requested > 0);

	std::cout << "requested " << requested << (requested == 1 ? " thread" : " threads") << std::endl;
	
	while (threads.size() < requested) 
		threads.emplace_back(std::make_unique<Thread>());

	assert(threads.size() == requested); 

}

void ThreadPool::start_searching() {

	for (auto& th : threads) {
		th.get()->start_searching();
	}
		
}	

void ThreadPool::set(Search::SearchLimits& limits) {

	for (auto& th : threads) {
		th->run_custom_job([&]() {
			th->worker->limits = limits;
		});
	};
}

} // namespace Engine
