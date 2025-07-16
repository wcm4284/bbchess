#pragma once

#include "position.h"

#include <string>

namespace Engine {

namespace Search {

class Worker {


	public:

		Worker() = default;

		void perft(int);
		void start_searching();



	private:
		
		void iterative_deepening(Position&);
		Value search(Position&, int, int, int, int);
		Value qsearch(Position&, int, int, int);

		Move pv_table[MAX_PLY][MAX_PLY];

};
	
	
} // namespace Search

} // namespace Engine
