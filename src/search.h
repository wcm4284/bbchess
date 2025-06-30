#pragma once

#include "position.h"

#include <string>

namespace Engine {

namespace Search {
	

	uint64_t perft(std::string, int);

	/* side effect of modifying pv_table
	   places moves found in its search in
	   pv_table, can be printed to find
	   the line it discovered was "best"
	*/
	void iterative_deepening(Position&, int);
		
	inline Move pv_table[MAX_PLY];
}

} // namespace Engine
