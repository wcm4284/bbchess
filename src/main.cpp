#include "types.h"

#include <iostream>
#include <bitset>

using namespace Engine;

int main() {

	Move m(65);

	std::cout << Square::SQ_H3 << std::endl;
	std::cout << std::bitset<6>(int(flip_file(SQ_H3))) << std::endl;
	std::cout << std::bitset<6>(int(SQ_A3)) << std::endl;

	return 0;

}
