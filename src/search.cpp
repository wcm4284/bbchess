#include "search.h"

namespace Engine {

int perft(Position& p, int depth) {

}

int perft(std::string fen, int depth) {

	Position p;
	p.set_fen(fen);

	uint64_t nodes = perft(p, depth);

	return nodes;

}



Move iterative_deepening(Position& p) {
	Move best_move;	
	return best_move;

}

} // namespace Engine
