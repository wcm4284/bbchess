#pragma once

#include "position.h"
#include "types.h"
#include "movegen.h"

#include <string>
#include <vector> 

namespace Engine {

class ThreadPool;

template <typename T>
class MoveQueue;

namespace Search {

struct SearchLimits {

	SearchLimits() : perft(0), depth(0) {}

	// depth for perft search
	Depth perft;
	// depth for regular search
	Depth depth;
};

struct PerftMove {

	PerftMove(Move m) : move(m), node_count(0) {}
	
	Move move; 
	uint64_t node_count;

};

using PerftMoves = std::vector<PerftMove>;


class Worker {


	public:

		Worker() = default;

		void start_searching();



	private:
		
		void iterative_deepening();
		void perft(MoveQueue< MoveList<LEGAL> >&);
		uint64_t perft(Position&, Depth); 
		Value search(Position&, int, int, int, int);
		Value qsearch(Position&, int, int, int);
	

		uint64_t nodes;

		SearchLimits limits;
		Position	 rootPos;
		Move pv_table[MAX_PLY][MAX_PLY];


	friend class Engine::ThreadPool;

};
	
	
} // namespace Search

} // namespace Engine
