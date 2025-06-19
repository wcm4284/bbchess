#include "types.h"
#include "bitboard.h"
#include "position.h"
#include "movegen.h"

using namespace Engine;

int main() {

	Bitboards::init();

	Position p;
	p.init();

	p.set_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

	std::cout << p << std::endl;

	//std::cout << Bitboards::pretty(shift<SOUTH>(square_bb(SQ_E3))) << std::endl;

	return 0;

}
