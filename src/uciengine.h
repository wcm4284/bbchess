#ifndef UCI_H_INCLUDED
#define UCI_H_INCLUDED

#include <sstream>

#include "engine.h"
#include "search.h"

namespace Engine {

class UCIEngine {

	public:
		UCIEngine() = default;
		UCIEngine(int, char**); 

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

#endif
