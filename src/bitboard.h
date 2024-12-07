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

constexpr Bitboard FileA = 0x0101010101;

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

} // namespace Engine

#endif
