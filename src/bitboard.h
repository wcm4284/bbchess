#pragma once
#ifndef BITBOARD_H_INCLUDED
#define BITBOARD_H_INCLUDED

#include <cassert>
#include <string>

#include "types.h"

namespace Engine {

namespace Bitboards {

void init();
std::string pretty(Bitboard b);

} // namespace Engine::Bitboards

constexpr Bitboard FileA = 0x0101010101010101ULL;
constexpr Bitboard FileB = FileA << 1;
constexpr Bitboard FileC = FileA << 2;
constexpr Bitboard FileD = FileA << 3;
constexpr Bitboard FileE = FileA << 4;
constexpr Bitboard FileF = FileA << 5;
constexpr Bitboard FileG = FileA << 6;
constexpr Bitboard FileH = FileA << 7;

constexpr Bitboard Rank1 = 0xFFULL;
constexpr Bitboard Rank2 = Rank1 << (8 * 1);
constexpr Bitboard Rank3 = Rank1 << (8 * 2);
constexpr Bitboard Rank4 = Rank1 << (8 * 3);
constexpr Bitboard Rank5 = Rank1 << (8 * 4);
constexpr Bitboard Rank6 = Rank1 << (8 * 5);
constexpr Bitboard Rank7 = Rank1 << (8 * 6);
constexpr Bitboard Rank8 = Rank1 << (8 * 7);

extern Bitboard Line[SQUARE_NB][SQUARE_NB];
extern Bitboard Between[SQUARE_NB][SQUARE_NB];
extern Bitboard PawnAttacks[COLOR_NB][SQUARE_NB];

constexpr Bitboard square_bb(Square s) {
	assert(is_ok(s));
	return (1ULL << s);
}

inline Bitboard operator&(Bitboard b, Square s) { return b & square_bb(s); }
inline Bitboard operator|(Bitboard b, Square s) { return b | square_bb(s); }
inline Bitboard operator^(Bitboard b, Square s) { return b ^ square_bb(s); }
inline Bitboard& operator&=(Bitboard& b, Square s) { return b &= square_bb(s); }
inline Bitboard& operator|=(Bitboard& b, Square s) { return b |= square_bb(s); }
inline Bitboard& operator^=(Bitboard& b, Square s) { return b ^= square_bb(s); }

inline Bitboard operator&(Square s, Bitboard b) { return b & s; }
inline Bitboard operator|(Square s, Bitboard b) { return b | s; }
inline Bitboard operator^(Square s, Bitboard b) { return b ^ s; }

constexpr bool more_than_one(Bitboard b) { return b & (b - 1); }


template <Direction d>
constexpr Bitboard shift(Bitboard b) {

	return   d == NORTH			? b << 8
		   : d == SOUTH			? b >> 8
		   : d == NORTH + NORTH ? b << 16
		   : d == SOUTH + SOUTH ? b >> 16
		   : d == EAST			? (b & ~FileH) << 1
		   : d == WEST			? (b & ~FileA) >> 1
		   : d == NORTH_EAST    ? (b & ~FileH) << 9
		   : d == NORTH_WEST    ? (b & ~FileA) << 7
		   : d == SOUTH_EAST    ? (b & ~FileH) >> 7
		   : d == SOUTH_WEST	? (b & ~FileA) >> 9
								: 0;
	
}

template <Color c>
constexpr Bitboard generate_pawn_attack(Bitboard b) {
	return c == WHITE ? shift<NORTH_EAST>(b) | shift<NORTH_WEST>(b)
					  : shift<SOUTH_EAST>(b) | shift<SOUTH_WEST>(b);
}

// TODO: magic numbers
// TODO: rook and bishop attacks

} // namespace Engine

#endif
