#include "search.h"
#include "movegen.h"
#include "evaluate.h"

namespace Engine {
using namespace Search;


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

Value negamax(Position& p, int depth) {

	Color us = p.to_play();

	if (depth == 0)
		return us == WHITE ? static_eval(p) : -static_eval(p);


	MoveList<LEGAL> ml = MoveList<LEGAL>(p);

	Value best_val = -VALUE_INFINITE;

	for (Move& m : ml) {
		p.do_move(&m);
		Value v = negamax(p, depth - 1);
		p.undo_move();

		if (v > best_val) {
			best_val = v;
			pv_table[p.ply()] = m;}
	}

	return best_val;
}

void search(Position& p, int depth) {

	MoveList<LEGAL> ml = MoveList<LEGAL>(p);

	Value b_val = -VALUE_INFINITE;

	for (Move& m : ml) {
		p.do_move(&m);
		Value v = negamax(p, depth - 1);
		p.undo_move();

		if (v > b_val) {
			b_val = v;
			pv_table[p.ply()] = m;}
	}

}
void Search::iterative_deepening(Position& p, int depth) {
	// populate pv table
	search(p, depth);
	
	std::cout << "Printing PV table:" << std::endl;
	for (int i = 0; i < depth; ++i) {
		std::cout << pv_table[i + p.ply()];}

}

} // namespace Engine
