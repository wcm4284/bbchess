#include "bitboard.h"
#include <string>

namespace Engine {

Bitboard PawnAttacks[COLOR_NB][SQUARE_NB];

void Bitboards::init() {

	for (Square s = SQ_A1; s <= SQ_H8; ++s) {

		PawnAttacks[WHITE][s] = generate_pawn_attack<WHITE>(square_bb(s));
		PawnAttacks[BLACK][s] = generate_pawn_attack<BLACK>(square_bb(s));

	}
	

}


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
