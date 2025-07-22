#include <sstream>

#include "search.h"
#include "evaluate.h"
#include "moveorder.h"

namespace Engine {


uint64_t Search::Worker::perft(Position& p, Depth depth) {

	assert(depth != 1);

	if (depth == 0)
		return 1;

	bool leaf = depth == 2;

	MoveList<LEGAL> moves(p);
	
	uint64_t cnt = 0;
	for (auto& m : moves) {

		p.do_move(&m);
		cnt += leaf ? MoveList<LEGAL>(p).size() : perft(p, depth - 1);
		p.undo_move(&m);

	}

	return cnt;
}

void Search::Worker::perft(PerftMoves& rootMoves) {
	
	assert(limits.perft);

	bool leaf = limits.perft == 2;

	Position p;
	p.set(rootPos.fen());

	Move *m;
	uint64_t cnt = 0;
	while ( (m = rootMoves.next()) ) {

		assert(m);
		
		p.do_move(m);
		nodes += cnt = leaf ? MoveList<LEGAL>(p).size() : perft(p, limits.perft - 1);
		p.undo_move(m);
	

		// this gives us a "race condition" in that the nodes will likely
		// not all be printed in the same order every time perft is run,
		// but the node count will remain the same and that is what is important
		std::stringstream out;
		out << p.dress_move(*m) << ": " << cnt << "\n";
		std::cout << out.str();

	}

}


Value Search::Worker::qsearch(Position& p, int alpha, int beta, [[maybe_unused]] int ply) {

	
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
	while ( (m = mo.next()) ) {

		p.do_move(m);
		Value val = -qsearch(p, -beta, -alpha, ply + 1);
		p.undo_move(m);
		
		if (val >= beta) 
			return beta;


		if ( val > best_val ) {
			best_val = val;		
			if ( val > alpha ) 
				alpha = val;
		
		}	
	}
	
	return best_val;
	
	return 0;
}

Value Search::Worker::search(Position& p, int alpha, int beta, Depth depth, int ply) {

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
	while ( (m = mo.next()) ) {
		p.do_move(m);
		Value v = -search(p, -beta, -alpha, depth - 1, ply + 1);
		p.undo_move(m);
		
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

	
	return 0;
}

void Search::Worker::iterative_deepening() {

	assert(limits.depth); 

	std::cout << "iterative deepening\n";

	int alpha = -VALUE_INF;
	int beta  =  VALUE_INF;
	Position     p;
	p.set(rootPos.fen());

	Value eval = search(p, alpha, beta, limits.depth, 0);

	std::cout << "best eval: " << eval << std::endl;
	std::cout << "1. " << p.dress_move(pv_table[0][0]) << std::endl;



}

void Search::Worker::start_searching() { 
	
	std::cout << "iterative deepening\n";
	std::cout << rootPos << std::endl;
	iterative_deepening(); 

}

} // namespace Engine
