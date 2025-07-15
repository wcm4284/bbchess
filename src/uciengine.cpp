#include <iostream>
#include <sstream>

#include "uciengine.h"

namespace Engine {

UCIEngine::UCIEngine() : engine(1) {}

void UCIEngine::loop() {

	std::string token, cmd;

	while (true) {

		std::getline(std::cin, cmd); 

		std::istringstream is(cmd); 

		is >> token;

		if (token == "quit" || token == "exit" || token == "stop")
			exit(1);


		if (token == "position") {
			position(is);
		} else if (token == "go") {
			go(is);
		} else {
			std::cout << "Unrecognized token " << token << ". Please try again.\n";
		}



	}
}

void UCIEngine::position(std::istringstream& is) {

	std::cout << "parsing position\n";

	std::string token;
	is >> token;

	if (token == "startpos") {
		//engine.set_position();
		is >> token; // get rid of "moves" that may follow
	} else if (token == "fen") {
		std::string fen;

		while (is >> token) {
			fen += token + " ";

		}

		std::cout << fen;

	}
}

void UCIEngine::go(std::istringstream& is) {
	std::cout << "parsing go\n";
}


}
