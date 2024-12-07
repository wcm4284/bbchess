#include "types.h"
#include "bitboard.h"

#include <iostream>
#include <bitset>

using namespace Engine;

int main() {

	std::cout << Bitboards::pretty(~FileH) << std::endl;

	std::cout << Bitboards::pretty(shift<SOUTH_EAST>(square_bb(SQ_E2))) << std::endl;

	std::cout << Bitboards::pretty(shift<NORTH_EAST>(square_bb(SQ_A1))) << std::endl;

	std::cout << Bitboards::pretty(shift<NORTH_WEST>(square_bb(SQ_H3))) << std::endl;

	std::cout << Bitboards::pretty(shift<SOUTH_WEST>(square_bb(SQ_E3))) << std::endl;

	return 0;

}
