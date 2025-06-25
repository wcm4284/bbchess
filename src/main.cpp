#include "types.h"
#include "bitboard.h"
#include "position.h"
#include "movegen.h"

using namespace Engine;

int main() {

	Bitboards::init();

	Position p;
	p.init();

	p.set_fen("r3k2r/pppppppp/8/8/8/4R3/PPPPPPPP/RNBQKBNR b KQkq - 0 1");

	MoveList<LEGAL> ml = MoveList<LEGAL>(p);
	std::cout << ml << std::endl;

	std::cout << p << std::endl;

	p.do_move(nullptr, &ml[19]);

	std::cout << p << std::endl;



	return 0;

}
