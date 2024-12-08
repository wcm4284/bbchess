#include "types.h"
#include "bitboard.h"

#include <iostream>
#include <bitset>

using namespace Engine;

int main() {

	Bitboards::init();

	std::cout << std::bitset<64>(Bitboard(0x1001011)) << std::endl;

	std::cout << std::bitset<64>(Bitboard(0x1011001)) << std::endl;

	std::cout << std::bitset<64>(pext(Bitboard(0x1001011), Bitboard(0x1011001))) << std::endl;

	std::cout << Bitboards::pretty(PseudoAttacks[KNIGHT][SQ_H8]) << std::endl;

	std::cout << Bitboards::pretty(PseudoAttacks[KING][SQ_H8]) << std::endl;

	return 0;

}
