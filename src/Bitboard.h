#pragma once
#ifndef BITBOARD_H_INCLUDED
#define BITBOARD_H_INCLUDED

#include <string>

namespace Engine {

namespace Bitboards {

void init();
std::string pretty(Bitboard b);

} // namespace Engine::Bitboards

constexpr Bitboard FileA = 0x0101010101




#endif
