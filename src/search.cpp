#include "search.h"
#include "evaluate.h"
#include "moveorder.h"

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

void Search::Worker::perft() {

	//uint64_t nodes = perft(p, depth, depth);
}



Value Search::Worker::qsearch(Position& p, int alpha, int beta, int ply) {

	
	Value eval = p.to_play() == WHITE ? evaluate(p) : -evaluate(p);

	if (eval >= beta)
		return beta;

	if (eval > alpha)
		alpha = eval;

	MoveOrder<CAPTURES> mo(p);
	
	// this doesn't deal with stalemate, which is technically a problem,
	// but until i'm faced with the scenario where it is i'll leave it
	if (mo.size() == 0)
		return eval;

	Value best_val = -VALUE_INF;
	Move* m;
	while ( (m = mo.next_move()) ) {

		p.do_move(m);
		Value val = -qsearch(p, -beta, -alpha, ply + 1);
		p.undo_move();
		
		if (val >= beta) 
			return beta;


		if ( val > best_val ) {
			best_val = val;		
			if ( val > alpha ) 
				alpha = val;
		
		}	
	}
	
	return best_val;
}

Value Search::Worker::search(Position& p, int alpha, int beta, int depth, int ply) {

	alpha = std::max(mated_in(ply), alpha);
	beta = std::min(mate_in(ply), beta);

	if (alpha >= beta)
		return alpha;

	if (depth == 0) 
		return qsearch(p, alpha, beta, ply);

	MoveOrder<LEGAL> mo(p);

	if (mo.size() == 0) 
		return p.checkers() ? mated_in(ply) : VALUE_DRAW;

	Value best_val = -VALUE_INF;	
	Move* m;
	while ( (m = mo.next_move()) ) {
		p.do_move(m);
		Value v = -search(p, -beta, -alpha, depth - 1, ply + 1);
		p.undo_move();
		
		if (v >= beta) 
			return beta;

		if (v > best_val) {

			best_val = v;

			if (v > alpha) {
				alpha = v;

				for (int i = 0; i < depth - 1; ++i) {
					pv_table[ply][i + 1] = pv_table[ply + 1][i];}

				pv_table[ply][0] = *m;
			}
		}
	}
	
	return best_val;
}

void Search::Worker::iterative_deepening(Position& p) {
	// TODO fix depth (the 1)
	Value score = search(p, -VALUE_INF, VALUE_INF, 1, 0);
	

	bool mate = (score >= VALUE_MATE_IN_MAX_PLY || score <= VALUE_MATED_IN_MAX_PLY);


/*
	if (score >= VALUE_MATE_IN_MAX_PLY)
		depth = VALUE_MATE - score;

	else if ( score <= VALUE_MATED_IN_MAX_PLY)
		depth = VALUE_MATE + score;
*/
	
	if (mate)
		std::cout << "mate ";

	std::cout << "pv ";
/*
	for (int i = 0; i < depth; ++i) 
		std::cout << (i + 1) << ". " << p.dress_move(pv_table[0][i]) << "\n";
		*/

}

void Search::Worker::start_searching() { 
	// are we doing perft or iterative deepening?
	std::cout << "search started\n";
}

} // namespace Engine
