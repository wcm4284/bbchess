#include "types.h"
#include "bitboard.h"

#include <iostream>
#include <bitset>

using namespace Engine;

int main() {

	std::cout << int(file_of(SQ_H8)) << FILE_H << std::endl;

	std::cout << Bitboards::pretty(square_bb(SQ_A1)) << std::endl;

	std::cout << Bitboards::pretty(square_bb(SQ_H3)) << std::endl;

	std::cout << Bitboards::pretty(square_bb(SQ_E5)) << std::endl;


	return 0;

}
