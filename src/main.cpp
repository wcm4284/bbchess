#include "types.h"
#include "bitboard.h"
#include "position.h"
#include "movegen.h"

using namespace Engine;

int main() {

	Bitboards::init();

	Position p;
	p.init();

	std::cout << Bitboards::pretty(attacks_bb<PAWN>(SQ_E3, BLACK)) << std::endl;

	return 0;

}
