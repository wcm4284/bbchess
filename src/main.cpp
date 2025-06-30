#include "types.h"
#include "bitboard.h"
#include "position.h"
#include "movegen.h"
#include "search.h"
#include "evaluate.h"

using namespace Engine;

int main() {

	Bitboards::init();

	std::string fen = "rbnqknbr/pppppppp/1P6/8/8/8/PPPPPP1P/RBNQKNBR KQkq - 0 1";
	
	Position p;
	p.set_fen(fen);
	std::cout << p << std::endl;
/*
	MoveList<LEGAL> ml = MoveList<LEGAL>(p);
	p.do_move(&ml[2]);
	std::cout << p << std::endl;

	ml = MoveList<LEGAL>(p);
	p.do_move(&ml[2]);
	std::cout << p << std::endl;

	ml = MoveList<LEGAL>(p);
	p.do_move(&ml[19]);
	std::cout << p << std::endl;

	ml = MoveList<LEGAL>(p);
	p.do_move(&ml[18]);
	std::cout << p << std::endl;

	ml = MoveList<LEGAL>(p);
	p.do_move(&ml[28]);
	std::cout << p << std::endl;

	ml = MoveList<LEGAL>(p);
	p.do_move(&ml[20]);
	std::cout << p << std::endl;
*/
	int d = 0;
	for (int i = 1; i <= d; ++i) {
		uint64_t nodes = perft(p.fen(), i);
		std::cout << "\nNodes searched: " << nodes << std::endl << std::endl;}

	std::cout << MoveList<LEGAL>(p) << std::endl;
	std::cout << MoveList<CAPTURES>(p) << std::endl;

	std::cout << iterative_deepening(p, 6) << std::endl;

	return 0;

}
