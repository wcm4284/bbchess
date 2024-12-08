#include "bitboard.h"

#include <string>

namespace Engine {

	
Bitboard Line[SQUARE_NB][SQUARE_NB];
Bitboard Between[SQUARE_NB][SQUARE_NB];
Bitboard PseudoAttacks[PIECE_TYPE_NB][SQUARE_NB];
Bitboard PawnAttacks[COLOR_NB][SQUARE_NB];


/*
Bitboard RookMovements[SQUARE_NB];
Bitboard BishopMovements[SQUARE_NB];

void generate_sliding_movements() {

	for (Square s = SQ_A1; s <= SQ_H8; ++s) {

		Bitboard edges = ((FileA | FileH) & ~file_bb(s)) | ((Rank1 | Rank8) & ~rank_bb(s));

		RookMovements[s] = ((rank_bb(s) | file_bb(s)) & ~edges) ^ s;

		// I can't think of a better way to do bishops than brute force
		Bitboard b = square_bb(s);

		Bitboard tmp = square_bb(s);

		while ((tmp = shift<NORTH_EAST>(tmp))) {
			b |= tmp;
		}
		tmp = square_bb(s);
		while ((tmp = shift<NORTH_WEST>(tmp))) {
			b |= tmp;
		}
		tmp = square_bb(s);
		while ((tmp = shift<SOUTH_EAST>(tmp))) {
			b |= tmp;
		}
		tmp = square_bb(s);
		while ((tmp = shift<SOUTH_WEST>(tmp))) {
			b |= tmp;
		}

		BishopMovements[s] = (b & ~edges) ^ s;

	}

}

*/

		

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
