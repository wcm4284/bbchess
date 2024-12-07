#include "types.h"
#include "bitboard.h"

#include <iostream>
#include <bitset>

using namespace Engine;

int main() {

	Bitboards::init();

	for (Color c : { WHITE, BLACK } ) {
		for (Square s = SQ_A1; s <= SQ_H8; ++s) {
			std::cout << Bitboards::pretty(PawnAttacks[c][s]) << "\n";
		}
	}

	return 0;

}
