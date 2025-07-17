#pragma once

#include <sstream>

#include "engine.h"
#include "search.h"

namespace Engine {

class UCIEngine {

	public:
		UCIEngine();

		void loop();


	private:
		void position(std::istringstream&);
		void go(std::istringstream&);
		Search::SearchLimits parse_limits(std::istringstream&);
		void perft();
		void start_searching();
		
		Engine engine;

};

} // namespace Engine
