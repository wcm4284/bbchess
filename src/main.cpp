#include "types.h"
#include "bitboard.h"
#include "position.h"
#include "movegen.h"

using namespace Engine;

int main() {

	Bitboards::init();

	Position p;
	p.init();

	MoveList<NON_EVASIONS> list = MoveList<NON_EVASIONS>(p, true);
	std::cout << list << std::endl;

	std::cout << p << std::endl;

	//std::cout << Bitboards::pretty(shift<SOUTH>(square_bb(SQ_E3))) << std::endl;

	return 0;

}
