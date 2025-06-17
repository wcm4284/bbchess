#include "types.h"
#include "bitboard.h"
#include "position.h"
#include "movegen.h"

using namespace Engine;

int main() {

	Bitboards::init();

	Position p;
	p.init();

	std::cout << Bitboards::pretty(attacks_bb(SQ_A1, square_bb(SQ_A2), QUEEN)) << std::endl;

	return 0;

}
