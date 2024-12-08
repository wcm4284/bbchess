#include "position.h"

#include <iostream>
#include <string_view>

namespace Engine {

namespace {

	std::string_view PieceToChar(" PNBRQK  pnbrqk");	
}
	
std::ostream& operator<<(std::ostream& os, const Position& pos) {

	os << "+---+---+---+---+---+---+---+---+\n";

	return os;

}



} // namespace Engine
