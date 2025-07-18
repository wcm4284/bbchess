#include <iostream>
#include <sstream>
#include <chrono>

#include "uciengine.h"

namespace Engine {


namespace {

	std::string startFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

}

UCIEngine::UCIEngine() : engine(2) {}

void UCIEngine::loop() {

	std::string token, cmd;

	do {

		std::getline(std::cin, cmd);
		std::istringstream is(cmd); 
		is >> token;

		if (token == "quit" || token == "exit")
			engine.stop();


		else if (token == "position") {
			position(is);
		} else if (token == "go") {
			go(is);
		} else {
			std::cout << "Unrecognized token '" << token << "'. Please try again.\n";
		}


	} while (token != "quit" && token != "exit");

}

void UCIEngine::position(std::istringstream& is) {

	std::string token;
	is >> token;

	std::string fen;

	if (token == "startpos") {
		fen = startFEN;
		is >> token; // get rid of "moves" that may follow
	} else if (token == "fen") {
		while (is >> token && token != "moves") {
			fen += token + " ";

		}
	}

	engine.set_position(fen);
}

void UCIEngine::go(std::istringstream& is) {

	auto start = std::chrono::high_resolution_clock::now();
		

	Search::SearchLimits limit = parse_limits(is);
	
	engine.set(limit);

	if (limit.perft)
		engine.perft();
	else	
		engine.go();

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	std::cout << "\nSearch took " << duration.count() << " ms\n\n";
	engine.reset();
}

Search::SearchLimits UCIEngine::parse_limits(std::istringstream& is) {

	Search::SearchLimits limit;
	

	std::string token;
	if (!(is >> token)) {
		return limit;
	}


	if (token == "perft") 
		is >> limit.perft;

	else if (token == "depth")
		is >> limit.depth;

	return limit;	

}


} // namespace Engine
