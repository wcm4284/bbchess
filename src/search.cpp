#include "search.h"
#include "movegen.h"

namespace Engine {

uint64_t perft(Position& p, int max, int depth) {

	if (depth == 0)
		return 1;

	MoveList<LEGAL> ml = MoveList<LEGAL>(p);

	uint64_t nodes = 0;

	for (Move m : ml) {
		if (depth == max)
			std::cout << p.dress_move(m) << ": ";
		p.do_move(&m);
		uint64_t new_nodes = perft(p, max, depth - 1);
		if (depth == max)
			std::cout << new_nodes << std::endl;
		nodes += new_nodes;
		p.undo_move();
	}

	return nodes;


}

uint64_t perft(std::string fen, int depth) {

	Position p;
	p.set_fen(fen);

	return perft(p, depth, depth);
}



Move iterative_deepening([[maybe_unused]] Position& p) {
	Move best_move;	
	return best_move;

}

} // namespace Engine
