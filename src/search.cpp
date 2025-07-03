#include "search.h"
#include "movegen.h"
#include "evaluate.h"

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

uint64_t Search::perft(std::string fen, int depth) {

	Position p;
	p.set_fen(fen);

	return perft(p, depth, depth);
}



Value qsearch(Position& p, int alpha, int beta) {

	++Search::searched;
	
	Value eval = p.to_play() == WHITE ? evaluate(p) : -evaluate(p);

	if (eval >= beta)
		return beta;

	if (eval > alpha)
		alpha = eval;

	MoveList<CAPTURES> ml = MoveList<CAPTURES>(p);

	if (ml.size() == 0)
		return eval;

	for (Move& m : ml) {

		++Search::extras;
		p.do_move(&m);
		Value val = -qsearch(p, -beta, -alpha);
		p.undo_move();
		
		if (val >= beta) 
			return beta;


		if ( val > alpha ) 
			alpha = val;
		
	}
	
	return alpha;
}

Value negamax(Position& p, int alpha, int beta, int depth, int ply) {

	if (depth == 0) {
		return qsearch(p, alpha, beta);}

	MoveList<LEGAL> ml = MoveList<LEGAL>(p);

	for (Move& m : ml) {
		p.do_move(&m);
		Value v = -negamax(p, -beta, -alpha, depth - 1, ply + 1);
		p.undo_move();
		
		if (v >= beta) 
			return beta;

		if (v > alpha) {
			alpha = v;

			for (int i = 0; i < depth - 1; ++i) {
				Search::pv_table[ply][i + 1] = Search::pv_table[ply + 1][i];}

			Search::pv_table[ply][0] = m;

		}
	}
	
	return alpha;
}

void Search::iterative_deepening(Position& p, int depth) {
	// populate pv table
	negamax(p, -VALUE_INF, VALUE_INF, depth, 0);
	
	std::cout << "Printing PV table:" << std::endl;
	for (int i = 0; i < depth; ++i) {
		std::cout << Search::pv_table[0][i];}

}

} // namespace Engine
