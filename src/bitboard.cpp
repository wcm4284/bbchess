#include "bitboard.h"

#include <string>

namespace Engine {
	
Bitboard Line[SQUARE_NB][SQUARE_NB];
Bitboard Between[SQUARE_NB][SQUARE_NB];
Bitboard PseudoAttacks[PIECE_TYPE_NB][SQUARE_NB];
Bitboard PawnAttacks[COLOR_NB][SQUARE_NB];

Magic Magics[SQUARE_NB][2];

namespace {

	Bitboard BishopTable[0x1480];
	Bitboard RookTable[0x19000];

void init_magics(PieceType pt, Bitboard table[], Magic magics[][2]);

Bitboard safe_dst(Square s, int step) {
	Square to = Square(s + step);
	return is_ok(to) && distance(s, to) <= 2 ? square_bb(to) : Bitboard(0);
}

Bitboard sliding_attacks(PieceType pt, Square s, Bitboard occupancy) {
	Bitboard attacks              = 0;
	Direction BishopDirections[4] = { NORTH_EAST, SOUTH_EAST, NORTH_WEST, SOUTH_WEST };
	Direction RookDirections[4]   = { NORTH, SOUTH, EAST, WEST };

	for (Direction d : (pt == ROOK ? RookDirections : BishopDirections)) {
		Square sq = s;
		while (safe_dst(sq, d)) {
			attacks |= (sq += d);
			if (sq & occupancy)
				break;
		}
	}

	return attacks;
}

} // anon namespace

void Bitboards::init() {
	init_magics(BISHOP, BishopTable, Magics);
	init_magics(ROOK, RookTable, Magics);

	for (Square s1 = SQ_A1; s1 <= SQ_H8; ++s1) {
		PseudoAttacks[WHITE][s1] = generate_pawn_attack<WHITE>(square_bb(s1));
		PseudoAttacks[BLACK][s1] = generate_pawn_attack<BLACK>(square_bb(s1));

		for (int step : { -17, -15, -10, -6, 6, 10, 15, 17 } ) 
			PseudoAttacks[KNIGHT][s1] |= safe_dst(s1, step);

		for (int step : { -9, -8, -7, -1, 1, 7, 8, 9 } ) 
			PseudoAttacks[KING][s1] |= safe_dst(s1, step);	

		PseudoAttacks[QUEEN][s1]  = PseudoAttacks[BISHOP][s1] = sliding_attacks(BISHOP, s1, 0);
		PseudoAttacks[QUEEN][s1] |= PseudoAttacks[ROOK][s1]   = sliding_attacks(ROOK, s1, 0);

		for (PieceType pt : { BISHOP, ROOK } ) {
			for (Square s2 = SQ_A1; s2 <= SQ_H8; ++s2) {
				if (PseudoAttacks[pt][s1] & s2) {
					Line[s1][s2] = sliding_attacks(pt, s1, square_bb(s2)) & sliding_attacks(pt, s2, square_bb(s1));
					Between[s1][s2] = Line[s1][s2] | s1 | s2;
				}
			}
		}
	}
}

std::string Bitboards::pretty(Bitboard b) {
	std::string s;
	
	for (Rank r = RANK_8; r >= RANK_1; --r) {
		s += "\n+---+---+---+---+---+---+---+---+\n";
		for (File f = FILE_A; f <= FILE_H; ++f) 
			s += (b & make_square(f, r)) ? "| X " : "|   ";
		
		s += "| " + std::to_string(1 + r);
	}

	return s += "\n+---+---+---+---+---+---+---+---+\n  a   b   c   d   e   f   g   h\n";
}

namespace {

void init_magics(PieceType pt, Bitboard table[], Magic Magics[][2]) {
	int size = 0;

#ifndef USE_PEXT
//TODO: actual magic numbers
	assert(false);
#endif

	for (Square s = SQ_A1; s <= SQ_H8; ++s) {
		Bitboard edges = ((FileA | FileH) & ~file_bb(s)) | ((Rank1 | Rank8) & ~rank_bb(s));

		Magic& m = Magics[s][pt - BISHOP];
		m.mask   = sliding_attacks(pt, s, 0) & ~edges;

		m.attacks = (s == SQ_A1) ? table : Magics[s - 1][pt - BISHOP].attacks + size;
		size      = 0;

		Bitboard b = 0;
		do {
			m.attacks[pext(b, m.mask)] = sliding_attacks(pt, s, b);
			b									= (b - m.mask) & m.mask;
			++size;
		} while (b);
	}
}

} // anon namespace

} // namespace Engine
