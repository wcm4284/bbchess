#include "types.h"
#include "bitboard.h"
#include "position.h"

#include <iostream>
#include <bitset>

using namespace Engine;

int main() {

	Bitboards::init();

	std::cout << Bitboards::pretty(Line[SQ_A1][SQ_A3]) << std::endl;

	std::cout << Bitboards::pretty(Between[SQ_A1][SQ_A3]) << std::endl;
	std::cout << Bitboards::pretty(Line[SQ_A1][SQ_B2]) << std::endl;


	return 0;

}
