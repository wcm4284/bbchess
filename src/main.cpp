#include "types.h"
#include "bitboard.h"
#include "position.h"
#include "movegen.h"

using namespace Engine;

int main() {

	Bitboards::init();

	Position p;
	p.init();

	std::cout << Bitboards::pretty(p.king_on(WHITE)) << std::endl;



	return 0;

}
