#include <iostream>
#include <sstream>

#include "uciengine.h"

namespace Engine {


namespace {

	std::string startFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

}

UCIEngine::UCIEngine() : engine(10) {}

void UCIEngine::loop() {

	std::string token, cmd;

	do {

		std::getline(std::cin, cmd);

		std::istringstream is(cmd); 

		is >> token;

		if (token == "quit" || token == "exit") {}


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
	
	
	std::cout << fen << "\n";

	engine.set_position(fen);
}

void UCIEngine::go(std::istringstream& is) {

	std::string token;
	if (!(is >> token))
		engine.go();

	if (token == "perft") {
		
		if (!(is >> token))
			engine.perft();
		else
			engine.perft(std::stoi(token));

	} else if (token == "depth") {
		
		is >> token;
		engine.go(std::stoi(token));

	}

}


} // namespace Engine
