#include "types.h"
#include "bitboard.h"
#include "position.h"
#include "movegen.h"

using namespace Engine;

int main() {

	Bitboards::init();

	Position p;
	p.init();

	p.set_fen("rnb1kbnr/pppppppp/8/4q3/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");

	MoveList<NON_EVASIONS> ml = MoveList<NON_EVASIONS>(p, true);

	std::cout << p << std::endl;
	std::cout << ml << std::endl;

	//std::cout << Bitboards::pretty(shift<SOUTH>(square_bb(SQ_E3))) << std::endl;

	return 0;

}
