#include "engine.h"

#include <string>

namespace Engine {

namespace {

std::string_view startFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

}

Engine::Engine(int nThreads) : threads( {tt} , nThreads), pos() { 
	pos.set(startFEN);
	tt.resize(5); 
}

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
