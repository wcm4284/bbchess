#include "types.h"
#include "bitboard.h"
#include "position.h"
#include "movegen.h"

using namespace Engine;

int main() {

	Bitboards::init();

	Position p;
//	p.init();

	p.set_fen("r3k1r1/pppppppP/8/8/8/8/8/RNBQKBNR w KQkq - 0 1");

	MoveList<LEGAL> ml = MoveList<LEGAL>(p);
	std::cout << ml << std::endl;

	std::cout << p << std::endl;

	p.do_move(&ml[3]);

	std::cout << p << std::endl;

	p.undo_move();

	std::cout << p << std::endl;

	return 0;

}
