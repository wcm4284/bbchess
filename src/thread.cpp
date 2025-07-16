#include <iostream>
#include <cassert>

#include "thread.h"

namespace Engine {

Thread::Thread() : th(&Thread::idle, this) {
	std::cout << "creating thread\n";


	this->worker = std::make_unique<Search::Worker>();
}

void Thread::idle() {

	std::cout << "idling thread\n";

}


ThreadPool::ThreadPool(size_t requested) {

	assert(requested > 0);

	std::cout << "requested " << requested << (requested == 1 ? "thread" : "threads") << std::endl;
	
	while (threads.size() < requested) 
		threads.emplace_back(std::make_unique<Thread>());

	assert(threads.size() == requested); 

}





}
