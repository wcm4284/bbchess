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

void Search::Worker::perft(PerftMoves& rootMoves, std::vector<int>& searchIndices) {
	
	assert(limits.perft);

	bool leaf = limits.perft == 2;

	Position p;
	p.set(rootPos.fen());


	for (auto& idx : searchIndices) {
		PerftMove& pm = rootMoves[idx];
		Move *m = &pm.move;
		
		p.do_move(m);
		nodes += pm.node_count = leaf ? MoveList<LEGAL>(p).size() : perft(p, limits.perft - 1); 
		p.undo_move(m);

	}

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
}

void Search::Worker::iterative_deepening() {

	std::cout << "iterative deepening\n";

}

void Search::Worker::start_searching() { 
	
	iterative_deepening(); 
}

} // namespace Engine
