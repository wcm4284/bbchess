#include "engine.h"

namespace Engine {

Engine::Engine(int nThreads) : threads(nThreads), pos() { pos.init(); }

void Engine::go() { 
	threads.start_searching();
}

void Engine::perft() {
	
	auto nodes = threads.perft(pos.fen());
	std::cout << "\nNodes searched: " << nodes << "\n" << std::endl;

}

void Engine::reset() { threads.clear(); }


void Engine::stop() { std::cout << "stopping\n"; }

}
