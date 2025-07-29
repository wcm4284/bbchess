#include "engine.h"

#include <string>

namespace Engine {

namespace {

std::string_view startFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

}

Engine::Engine(int nThreads) : threads( {tt}, nThreads), pos() { 
    pos.set(startFEN);
    tt.resize(5); 
}

void Engine::go() { 
    tt.set_probe(ProbeType::SEARCH);
    threads.start_searching();
}

void Engine::perft() {
    tt.set_probe(ProbeType::PERFT);
    auto nodes = threads.perft(pos.fen());
    std::cout << "\nNodes searched: " << nodes << "\n" << std::endl;
}

void Engine::reset() { threads.clear(); }


void Engine::stop() { std::cout << "stopping\n"; }

} // namespace Engine
