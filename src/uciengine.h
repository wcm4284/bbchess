#pragma once

#include <sstream>

#include "engine.h"

namespace Engine {

class UCIEngine {

	public:
		UCIEngine();

		void loop();


	private:
		void position(std::istringstream&);
		void go(std::istringstream&);
		void perft();
		void start_searching();
		
		Engine engine;

};

} // namespace Engine
