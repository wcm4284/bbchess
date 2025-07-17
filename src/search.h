#pragma once

#include "position.h"

#include <string>

namespace Engine {

class ThreadPool;

namespace Search {

struct SearchLimits {

	uint8_t depth;
	bool perft;
};

class Worker {


	public:

		Worker() = default;

		void start_searching();



	private:
		
		void iterative_deepening(Position&);
		void perft();
		Value search(Position&, int, int, int, int);
		Value qsearch(Position&, int, int, int);
	

		SearchLimits limits;
		Move pv_table[MAX_PLY][MAX_PLY];

	friend class Engine::ThreadPool;

};
	
	
} // namespace Search

} // namespace Engine
