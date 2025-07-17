#include "engine.h"

namespace Engine {

Engine::Engine(int nThreads) : threads(nThreads), pos() {}

void Engine::go(int depthLimit) {

	std::cout << "go with depthLimit " << depthLimit << std::endl;
	threads.start_searching();
}

void Engine::stop() { std::cout << "stopping\n"; }

}
