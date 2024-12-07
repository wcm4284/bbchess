#include "bitboard.h"
#include <string>

namespace Engine {


std::string Bitboards::pretty(Bitboard b) {

	std::string s;
	
	for (Rank r = RANK_8; r >= RANK_1; --r) {

		s += "\n+---+---+---+---+---+---+---+---+\n";
		
		for (File f = FILE_A; f <= FILE_H; ++f) {

			s += (b & make_square(f, r)) ? "| X " : "|   ";

		}

		s += "| " + std::to_string(1 + r);

	}

	return s += "\n+---+---+---+---+---+---+---+---+\n  a   b   c   d   e   f   g   h\n";

}

} // namespace Engine
