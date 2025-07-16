#include "engine.h"

namespace Engine {

Engine::Engine(int nThreads) : threads(nThreads), pos() {}

void Engine::perft(int depthLimit) {

	std::cout << "perft with depthLimit " << depthLimit << std::endl;
}

void Engine::go(int depthLimit) {
	std::cout << "go with depthLimit " << depthLimit << std::endl;
}


}
