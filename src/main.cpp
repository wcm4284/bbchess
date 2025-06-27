#include "types.h"
#include "bitboard.h"
#include "position.h"
#include "movegen.h"
#include "search.h"

using namespace Engine;

int main() {

	Bitboards::init();

	std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	
	Position p;
	p.init();
/*
	MoveList<LEGAL> ml = MoveList<LEGAL>(p);
	p.do_move(&ml[3]);

	std::cout << p << std::endl;

	ml = MoveList<LEGAL>(p);
	p.do_move(&ml[4]);

	std::cout << p << std::endl;

	ml = MoveList<LEGAL>(p);
	p.do_move(&ml[0]);

	std::cout << p << std::endl;

	ml = MoveList<LEGAL>(p);
	p.do_move(&ml[21]);

	std::cout << p << std::endl;
*/
	int d = 5;
	uint64_t nodes = perft(p.fen(), d);
	std::cout << "\nDepth: " << d << ", Nodes: " << nodes << std::endl << std::endl;


	return 0;

}
