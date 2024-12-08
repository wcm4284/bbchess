#include "types.h"
#include "bitboard.h"

#include <iostream>

using namespace Engine;

int main() {

	Bitboards::init();

	std::cout << std::hex << pext(Bitboard(0x1001011), Bitboard(0x1011001)) << std::endl;

	return 0;

}
