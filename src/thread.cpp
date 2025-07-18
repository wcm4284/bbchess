#include <iostream>
#include <cassert>

#include "thread.h"
#include "movegen.h"

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
}

void Thread::start_searching() {
	assert(worker);
	run_custom_job([this] { worker->start_searching(); });
}

void Thread::idle() {

	while (true) {
		searching = false;	
		cv.notify_one();
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

	std::cout << "requested " << requested << (requested == 1 ? " thread" : " threads") << "\n";

	assert(requested > 0);
	
	while (threads.size() < requested) 
		threads.emplace_back(std::make_unique<Thread>());

	assert(threads.size() == requested); 

}

void ThreadPool::clear() {

	for (auto& th : threads) {
		th->run_custom_job([&] {
			th->worker->nodes = 0;
		});
	}

	wait_for_all_threads();
}

void ThreadPool::start_searching() {

	for (auto& th : threads) {
		th.get()->start_searching();
	}
		
}	

void ThreadPool::set(Search::SearchLimits& limits, Position& pos) {
	
	for (auto& th : threads) {
		th->run_custom_job([&]() {
			th->worker->limits = limits;
			th->worker->rootPos = pos;
		});

	};

	wait_for_all_threads();

}

void ThreadPool::wait_for_all_threads() {

	for (auto& th : threads)
		th->wait_for_job_finished();

}

uint64_t ThreadPool::perft(std::string fen) {
	

	Position        p;
	p.set(fen);
	MoveList<LEGAL> moves(p);

	Search::PerftMoves rootMoves;

	for (auto m : moves)
		rootMoves.emplace_back(m);

	std::vector<std::vector<int>> searchIndices;
	searchIndices.resize(threads.size());

	for (size_t i = 0; i < rootMoves.size(); ++i) 
		searchIndices[i % threads.size()].push_back(i);

	for (size_t i = 0; i < threads.size(); ++i) 
		threads[i]->run_custom_job([&, i] { threads[i]->worker->perft(rootMoves, searchIndices[i]); });

	wait_for_all_threads();

	for (auto m : rootMoves) {
		std::cout << p.dress_move(m.move) << ": " << m.node_count << "\n";	
	}

	return accumulate(&Search::Worker::nodes);
}

} // namespace Engine
